/*
 * < Kawabonga ! >
 * ------------------------------
 *    \   ^__^
 *     \  (oo)\_______
 *        (__)\       )\/\
 *            ||----w |
 *            ||     ||
 * 
 */

#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <exception>
#include <string>

namespace ftp
{
	class SocketCreationError: public std::exception
	{
		private:
			const std::string _err;

		public:
			SocketCreationError(const std::string &e): _err(e) {}
			const char *what () const noexcept override
			{
				return _err.c_str();
			}
	};

	class BindError: public std::exception
	{
		private:
			const std::string _err;

		public:
			BindError(const std::string &e): _err(e) {}
			const char *what () const noexcept override
			{
				return _err.c_str();
			}
	};

	class ListenError: public std::exception
	{
		private:
			const std::string _err;

		public:
			ListenError(const std::string &e): _err(e) {}
			const char *what () const noexcept override
			{
				return _err.c_str();
			}
	};

	class PollError: public std::exception
	{
		private:
			const std::string _err;

		public:
			PollError(const std::string &e): _err(e) {}
			const char *what () const noexcept override
			{
				return _err.c_str();
			}
	};

};

#endif // ERRORS_HPP