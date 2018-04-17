/**
 * \brief Wavefront OBJ and MTL parser implementation
 * \file
 */
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include "objparser.h"

std::string ObjParser::getPath(const std::string &filename)
{
	// Check for last / character
	std::string::size_type pos = filename.rfind('/');

	// If not found, check for the last \ character
	if (pos == std::string::npos)
		pos = filename.rfind('\\');

	// If not found, return "." (current directory).
	if (pos == std::string::npos)
		return ".";

	// Otherwise get path up to the separator
	return filename.substr(0, pos);
}

bool ObjParser::loadMaterialLib(const std::string &filename)
{
	std::ifstream is((basePath + "/" + filename).c_str());

	if (!is.is_open())
	{
		std::cerr << "ObjParser::loadMaterialLib(): Unable to open material library " << basePath << " / " << filename << " for reading." << std::endl;
		return false;
	}

	std::string materialName = "";
	std::string line;
	while (std::getline(is, line))
	{
		// Remove end of the line if a comment character is found
		std::string::size_type commentPos = line.find('#');
		if (commentPos != std::string::npos)
			line = line.substr(0, commentPos);

		// Skip empty lines
		if (line == "")
			continue;

		std::string firstToken;
		std::stringstream ss(line);
		ss >> firstToken;

		// Does this start a new material?
		if (firstToken == "newmtl")
		{
			// Yes.
			ss >> materialName;
//			std::cout << "Adding new material " << materialName << std::endl;
			continue;
		}

		// If we have a valid material, let it parse the data
		if (materialName != "")
			matlib[materialName].parseLine(line);
	}

	return true;
}

void ObjParser::Material::parseLine(const std::string &mtl_line)
{
	// Parse line as string tokens.
	std::stringstream ss(mtl_line);

	std::vector<std::string> infoVec;
	std::string nameStr;

	std::string token;
	while (ss >> token)
	{
		// Don't process empty strings (at end of line)
//		if (token.size() == 0)
//			continue;

		// Store first string as key for this data. The rest form the value
		if (nameStr == "")
			nameStr = token;
		else
			infoVec.push_back(token);
	}

	// Store material info
	info[nameStr] = infoVec;
}

/**
* \brief Get material value as a vector of strings
*
* \param key Material key name. i.e. "map_Kd" for diffuse texture map.
* \param[out] val Value as a string
* \return true if success, false if key was not found
*/
bool ObjParser::Material::get(const std::string &key, std::vector<std::string> &val) const
{
	std::map<std::string, std::vector<std::string> >::const_iterator it = info.find(key);

	if (it == info.end())
		return false;

	// Store value
	val = it->second;

	return true;
}

/**
* \brief Get material value as a string
*
* \param key Material key name. i.e. "map_Kd" for diffuse texture map.
* \param[out] val Value as a string
* \return true if success, false if key was not found
*/
bool ObjParser::Material::get(const std::string &key, std::string &val) const
{
	std::map<std::string, std::vector<std::string> >::const_iterator it = info.find(key);

	if (it == info.end())
		return false;

	// Check that we have enough values for this key
	if (it->second.size() == 0)
		return false;

	// Store the last value of the vector (in case there are options which aren't really supported by this implementation)
	val = it->second[it->second.size() - 1];

	return true;
}

/**
* \brief Get material value as a float. 
*
* \param key Material key name. i.e. "Ns" for specular exponent
* \param[out] val Value as a float
* \return true if success, false if key was not found
*/
bool ObjParser::Material::get(const std::string &key, float &val) const
{
	std::map<std::string, std::vector<std::string> >::const_iterator it = info.find(key);

	if (it == info.end())
		return false;
	
	if (it->second.size() == 0)
		return false;

	// Use the last value of the vector in case there are options which aren't really supported by this implementation.
	std::stringstream ss(it->second[it->second.size() - 1]);
	ss >> val;

	return ss.good();
}

/**
* \brief Get material value as a 3 long vector
*
* \param key Material key name. i.e. "Ka" for constant ambient color.
* \param[out] val Value as a float
* \return true if success, false if key was not found
*/
bool ObjParser::Material::get(const std::string &key, glm::vec3 &val) const
{
	std::map<std::string, std::vector<std::string> >::const_iterator it = info.find(key);

	if (it == info.end())
		return false;

	// We expect a 3 long vector
	if (it->second.size() != 3)
		return false;

	// Parse three float values
	for (glm::vec3::length_type i = 0; i < 3; ++i)
	{
		std::stringstream ss(it->second[i]);
		ss >> val[i];
	}

	return true;
}

/**
 * \brief Return index of a vertex buffer with requested capabilities (or create a new one)
 */
unsigned int ObjParser::VertexSet::getVertexBufferIndex(bool hasTexture, bool hasNormal)
{
	for (size_t i = 0; i < vertexBuffers.size(); ++i)
	{
		if ((hasTexture == vertexBuffers[i].hasTexture()) &&
			(hasNormal == vertexBuffers[i].hasNormal()))
			return static_cast<unsigned int>(i);
	}

	// Create a new vertexBuffer for this one
	vertexBuffers.push_back(VertexBuffer());
	return static_cast<unsigned int>(vertexBuffers.size() - 1);
}

unsigned int ObjParser::VertexBuffer::addVertex(const glm::vec4 *pos, const glm::vec2 *texture, const glm::vec3 *normal)
{
	const bool compress_vertices = true;

	if (compress_vertices)
	{
		// Find out if given vertex information exists already as exact copy.
		// This is slow but should reduce memory use if exactly the same information is used in several faces.
		for (unsigned int i = 0; i < this->pos.size(); ++i)
		{
			if (this->pos[i] != *pos)
				continue;
			if (texture && this->texture[i] != *texture)
				continue;
			if (normal && this->normal[i] != *normal)
				continue;

			// Everything matches
			return i;
		}
	}

	// Verify that pos, texture and normal (if used) have the same size.
	if (texture && this->texture.size() != this->pos.size())
		throw std::runtime_error("ObjParser::VertexBuffer::addVertex(): Trying to add a vertex with texture information to a buffer with samples without texture information!");
	if (normal && this->normal.size() != this->pos.size())
		throw std::runtime_error("ObjParser::VertexBuffer::addVertex(): Trying to add a vertex with normal information to a buffer with samples without normal information!");

	// Add a new element to the end of the list
	this->pos.push_back(*pos);
	if (texture)
		this->texture.push_back(*texture);
	if (normal)
		this->normal.push_back(*normal);

	// Return index to the last element
	return static_cast<unsigned int>(this->pos.size() - 1);
}


/**
* \brief Clear current data.
*/
void ObjParser::VertexSet::clear()
{
	// We swap with a temporary object to release backing buffers completely.
	{
		std::vector<VertexBuffer> tmp;
		vertexBuffers.swap(tmp);
	}
	groupMaterialFaces.clear();
}


bool ObjParser::load(const std::string &objfile)
{
	std::ifstream is(objfile.c_str());

	if (!is.is_open())
		return false;

	// Remove previous materials
	matlib.clear();

	// Set base path for other file references with the object filename
	basePath = getPath(objfile);

	// Individual vectors for position, texture and normals
	std::vector<glm::vec4> posVec;
	std::vector<glm::vec2> textureVec;
	std::vector<glm::vec3> normalVec;

	std::string objectName = "";
	std::string groupName = "";
	std::string materialName = "";
	VertexSet curVertexSet;
	int curVertexBufferIndex = -1; // Negative values for "not selected yet"
	IndexRange curIndexRange; // Range in vertex buffers for current part (if curVertexBufferIndex is >= 0)

	// Parse file line by line
	std::string line;
	unsigned int lineno = 0;
	while (std::getline(is, line))
	{
		++lineno;

		// Remove end of the line if a comment character is found
		std::string::size_type commentPos = line.find('#');
		if (commentPos != std::string::npos)
			line = line.substr(0, commentPos);

		if (line == "")
			continue;

		std::string firstToken;
		std::stringstream ss(line);
		ss >> firstToken;

		// What kind of data is this?
		if (firstToken == "mtllib")
		{
			// Defines a material library for this object. Multiple libraries can be defined.
			std::string mtlfile;
			while (ss.good())
			{
				ss >> mtlfile;

				if (mtlfile == "")
					continue;

//				std::cout << "Loading material library " << mtlfile << std::endl;
				loadMaterialLib(mtlfile);
			}
		} else
		if (firstToken == "usemtl")
		{
			// Material changing. Current vertex buffer system no longer valid.
			if (curVertexBufferIndex >= 0)
				curVertexSet.addRange(groupName, materialName, curIndexRange);
			curVertexBufferIndex = -1;

			ss >> materialName;
//			std::cout << "Selected material " << materialName << std::endl;

		}
		else
		if (firstToken == "o")
		{
			// Object changing. Current vertex buffer system no longer valid.
			if (curVertexBufferIndex >= 0)
				curVertexSet.addRange(groupName, materialName, curIndexRange);
			curVertexBufferIndex = -1;

			// If we have existing data, store it
			if (curVertexSet.groupMaterialFaces.size() != 0)
			{
				// Do (deep) copy of current vertex. It is now complete.
				objVertexSet[objectName] = curVertexSet;
				curVertexSet.clear();
			}

			// Start defining a new object. Also resets vertex group name to empty string.
			// Objects are parsed as independent components that do not reuse data.
			ss >> objectName;
			groupName = "";

//			std::cout << "Starting to read new object " << objectName << std::endl;
		} else
		if (firstToken == "g")
		{
			// Group changing. Current vertex buffer system no longer valid.
			if (curVertexBufferIndex >= 0)
				curVertexSet.addRange(groupName, materialName, curIndexRange);
			curVertexBufferIndex = -1;

			// Start defining a new object group
			ss >> groupName;
//			std::cout << "Starting new object group " << groupName << std::endl;
		} else
		if (firstToken == "v")
		{
			// Start defining a new vertex position
			// Read rest of the line
			glm::vec4 pos;
			pos.w = 1.0f;
			ss >> pos.x >> pos.y >> pos.z >> pos.w; // The last one will fail if it's not available. That's ok.
			posVec.push_back(pos);
//			std::cout << "New vertex position: " << pos.x << ", " << pos.y << ", " << pos.z << ", " << pos.w << " -> " << posVec.size() << std::endl;
		} else
		if (firstToken == "vt")
		{
			// Start defining a new vertex texture coordinates
			// Read rest of the line
			glm::vec2 texture;
			ss >> texture.x >> texture.y;
			textureVec.push_back(texture);
//			std::cout << "New vertex texture position: " << texture.x << ", " << texture.y << " -> " << textureVec.size() << std::endl;
		}
		else
		if (firstToken == "vn")
		{
			// Start defining a new vertex normal vector
			glm::vec3 normal;
			ss >> normal.x >> normal.y >> normal.z;
			normalVec.push_back(normal);
//			std::cout << "New vertex normal vector: " << normal.x << ", " << normal.y << ", " << normal.z << " -> " << normalVec.size() << std::endl;
		} else
		if (firstToken == "f")
		{
			// Start defining a new face
			std::vector<unsigned int> f;

//			std::cout << "Processing " << firstToken << std::endl;

			// Parse as strings first
			std::string token;
			while (ss.good())
			{
				ss >> token;
				if (token == "")
					continue;

				// Split token with / character
//				std::cout << "Parsing " << token << std::endl;
				std::stringstream ts(token);
				std::string part;
				std::vector<int> indices;
				while (std::getline(ts, part, '/'))
				{
					std::stringstream ps(part);
					int i = 0;
					ps >> i; // If stream ends prematurely as it is not empty, i is not updated
					indices.push_back(i);
//					std::cout << "Parsed " << i << std::endl;
				}

				// Fill missing values with zeros. Zero is not a valid index in Wavefront Object model.
				while (indices.size() < 3)
					indices.push_back(0);

				// Replace negative values with index value calculated from the end
				if (indices[0] < 0)
					indices[0] = static_cast<int>(posVec.size()) + indices[0];
				if (indices[1] < 0)
					indices[1] = static_cast<int>(textureVec.size()) + indices[1];
				if (indices[2] < 0)
					indices[2] = static_cast<int>(normalVec.size()) + indices[2];

				// Get a vertex buffer index if we don't have a valid one yet.
				bool hasTexture = indices[1] > 0;
				bool hasNormal = indices[2] > 0;
				if (curVertexBufferIndex < 0)
				{
					curVertexBufferIndex = curVertexSet.getVertexBufferIndex(hasTexture, hasNormal);

					// Initialize curIndexRange structure for this face sequence. Starting index and lengths are currently unknown.
					curIndexRange.vbIndex = curVertexBufferIndex;
					curIndexRange.startIndex = 0;
					curIndexRange.length = 0;
				}

				// Store vertex index
				f.push_back(curVertexSet.addVertex(
					curVertexBufferIndex,
					&posVec.at(indices[0] - 1),
					hasTexture ? &textureVec.at(indices[1] - 1) : 0,
					hasNormal ? &normalVec.at(indices[2] - 1) : 0
					));
			}

			// Update curIndexRange with the face data
			if (curIndexRange.length == 0)
			{
				// First face. This determines starting index in the sequence
				curIndexRange.startIndex = curVertexSet.addVertexIndex(curVertexBufferIndex, f[0]);
				for (size_t i = 1; i < f.size(); ++i)
					curVertexSet.addVertexIndex(curVertexBufferIndex, f[i]);
			} else
			{
				// One of the subsequent faces. Just add the indices
				for (size_t i = 0; i < f.size(); ++i)
					curVertexSet.addVertexIndex(curVertexBufferIndex, f[i]);
			}
			// Increase the range of valid indices
			curIndexRange.length += static_cast<unsigned int>(f.size());
		} else
		if (firstToken == "s")
		{
			// Smooth shading on/off. Not supported. Just export normals for the models directly.
		} else
		{
			std::cerr << "ObjParser::load(): " << objfile << ":" << lineno << ": Ignoring unknown token \"" << firstToken << "\"" << std::endl;
		}
	}

	// If we have existing data, store it
	if (curVertexBufferIndex >= 0)
		curVertexSet.addRange(groupName, materialName, curIndexRange);
	curVertexBufferIndex = -1;

	if (curVertexSet.groupMaterialFaces.size() != 0)
	{
		// Do (deep) copy of current vertex. It is now complete.
		objVertexSet[objectName] = curVertexSet;
		curVertexSet.clear();
	}

	return true;
}