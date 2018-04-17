/**
 * \file Stream Redirection class
 */
#ifndef STREAM_REDIRECTOR_H_
#define STREAM_REDIRECTOR_H_

#include <string>
#include <iostream>
#include <fstream>

/**
* \brief Redirect stdout and stderr to files instead of actual output.
*
* This functionality is useful on Windows for generating debugging output easily as we don't have a console to work with.
* Just create an instance of this class in main() to start redirecting.
*/
class StreamRedirector
{
	std::ofstream sout;
	std::ofstream eout;
	std::streambuf *coutbuf;
	std::streambuf *cerrbuf;
public:
	/**
	 * \brief Constructor
	 * \param stdout_file File to use for standard output
	 * \param stderr_file File to use for standard error
	 */
	StreamRedirector(const std::string &stdout_file = "stdout.txt", const std::string &stderr_file = "stderr.txt") :
		sout(stdout_file.c_str()),
		eout(stderr_file.c_str()),
		coutbuf(0),
		cerrbuf(0)
	{
		// Redirect streams if we could open defined output files
		if (sout.is_open())
		{
			coutbuf = std::cout.rdbuf(); //save old buf
			std::cout.rdbuf(sout.rdbuf()); //redirect std::cout to stdout_file
		}

		if (eout.is_open())
		{
			cerrbuf = std::cerr.rdbuf(); //save old buf
			std::cerr.rdbuf(eout.rdbuf()); //redirect std::err to stderr_file
		}
	}

	/**
	 * \brief Destructor
	 */
	~StreamRedirector()
	{
		// Restore old stdout, stderr buffers
		if (coutbuf)
			std::cout.rdbuf(coutbuf);
		if (cerrbuf)
			std::cerr.rdbuf(cerrbuf);
	}
};

#endif
