/*
 * \brief Wavefront OBJ (and .mtl) file parser interface
 * \file
 */
#ifndef OBJ_PARSER_H_
#define OBJ_PARSER_H_

#include <string>
#include <vector>
#include <map>
#include <glm/glm.hpp>

// Disable warning of too long decorated names in Visual Studio due to deep template recursion of ObjParser::VertexSet::groupMaterialFaces
#ifdef _MSC_VER
#pragma warning(disable : 4503)
#endif

class ObjParser
{
	std::string getPath(const std::string &filename);
	bool loadMaterialLib(const std::string &filename);

public:
	/**
	 * \brief Wavefront Object material information
	 */
	class Material
	{
		std::map<std::string, std::vector<std::string> > info; ///< Material information in string form
	public:
		Material() { }

		// Add new information by parsing material lines
		void parseLine(const std::string &mtl_line);

		// Get material properties as different types
		bool get(const std::string &key, std::vector<std::string> &val) const;
		bool get(const std::string &key, std::string &val) const;
		bool get(const std::string &key, float &val) const;
		bool get(const std::string &key, glm::vec3 &val) const;
	};

	/**
	 * \brief Vertex Buffer mapping
	 *
	 * Objects of different materials in VertexSet map to indexes in VertexBuffer if they
	 * share the same attributes. i.e. they either have or don't have texture and normal values provided.
	 * This means that they can be potentially drawn without changing too many OpenGL state variables between them.
	 */
	class VertexBuffer
	{
	public:
		std::vector<glm::vec4> pos; // Vertex coordinates (always available)
		std::vector<glm::vec2> texture; // Texture (uv) coordinates
		std::vector<glm::vec3> normal; // Vertex normal vectors

		bool hasPos() const { return pos.size() > 0; }
		bool hasTexture() const { return texture.size() > 0; }
		bool hasNormal() const { return normal.size() > 0; }

		std::vector<unsigned int> indices;

		unsigned int addVertex(const glm::vec4 *pos = 0, const glm::vec2 *texture = 0, const glm::vec3 *normal = 0);
	};

	/**
	 * \brief Tells where to find the indices for rendering a mesh
	 */
	class IndexRange
	{
	public:
		unsigned int vbIndex; ///< Vertex Buffer Index (VertexSet.vertexBuffers)
		unsigned int startIndex; ///< Where the indexes matching this object part starts in VertexBuffer.indices;
		unsigned int length; ///< The length of this object part in VertexBuffer.indices.

		IndexRange() {}
	};

	/**
	 * \brief A set of vertex information that is used by a single object
	 *
	 * If texture or normal coordinates are not available, their vectors are empty.
	 * Original format can compress situations where same positions are used with different textures or normals.
	 */
	class VertexSet
	{
	public:
		std::vector<VertexBuffer> vertexBuffers;

		// Faces are divided to object groups and materials
		// groupMaterialFaces[group name][material name][face index][face vertex id] indexes pos, texture and normal vectors

		typedef std::map<std::string, std::map<std::string, std::vector<IndexRange> > > group_type;
		typedef std::map<std::string, std::vector<IndexRange> > material_type;
		typedef std::vector<IndexRange> faces_type;

		group_type groupMaterialFaces; 
	
		VertexSet()	{}

		// Pick vertex buffer index that has the matching capabilities with indexed vertices.
		unsigned int getVertexBufferIndex(bool hasTexture, bool hasNormal);

		// Add a new vertex into a selected vertex buffer. Returns an index in that buffer
		unsigned int addVertex(int vertexBufferIndex, const glm::vec4 *pos = 0, const glm::vec2 *texture = 0, const glm::vec3 *normal = 0)
		{
			return vertexBuffers[vertexBufferIndex].addVertex(pos, texture, normal);
		}

		unsigned int addVertexIndex(int vertexBufferIndex, unsigned int vindex)
		{
			vertexBuffers[vertexBufferIndex].indices.push_back(vindex);
			return static_cast<unsigned int>(vertexBuffers[vertexBufferIndex].indices.size() - 1); // Return position of the added index.
		}

		/**
		 * \brief Add a new face to vertex set.
		 * \param groupName Object group name
		 * \param materialName Material name to use for this face
		 * \param f Information where vertex indices can be found. Refers to data in vertexBuffers;
		 */
		void addRange(const std::string &groupName, const std::string &materialName, const IndexRange &f)
		{
			groupMaterialFaces[groupName][materialName].push_back(f); 
		}

		void clear();
	};

	// All object information
	/////////////////////////
	std::string basePath;
	std::map<std::string, Material> matlib; ///< Material library for the object
	std::map<std::string, VertexSet > objVertexSet; ///< objVertexSet[object name] = Vertex set

	ObjParser()	{ }

	ObjParser(const std::string &objfile)
	{
		load(objfile);
	}

	bool load(const std::string &objfile);
};

#endif