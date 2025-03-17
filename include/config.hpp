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

#ifndef CONFIG_HPP
#define CONFIG_HPP

namespace ftp
{
	namespace cfg {
		const int MAXQUEUESIZE = 1001; // max queue size of the server
		const int POLLTIMEOUT = -1; // poll() timeout in milliseconds
		const int RTRYPASV = 10; // number of tries to bind to a random port for PASV
	}

	namespace responses {
		//BASE
		constexpr const char *WELCOME = "220 Welcome to My FTP Server\r\n";
		constexpr const char *NOT_LOGGED_IN = "530 Not logged in.\r\n";
		constexpr const char *TIMEOUT = "421 Connection timed out.\r\n";
		constexpr const char *FAIL = "451 Couldn't fulfill this request.\r\n";
		constexpr const char *BADCMD = "500 Invalid command given, try HELP.\r\n";

		//PASV
		constexpr const char *PASV = "227 Entering passive mode.";

		//RETR
		constexpr const char *DATACONN_OPEN = "150 Opening data connection.\r\n";
		constexpr const char *DATACONN_CLOSE = "226 Closing data connection.\r\n";
		constexpr const char *DATACONN_FAIL = "425 Can't open data connection.\r\n";
		constexpr const char *NOFILE_SPEC = "550 No file specified.\r\n";
		constexpr const char *NOFILE_FOUND = "550 File not found.\r\n";
		constexpr const char *TRANSFER_COMPLETE = "226 Transfer complete.\r\n";

		//USER
		constexpr const char *USER_KO = "331 Username incorrect.\r\n";
		constexpr const char *USER_OK = "331 User name okay, need password.\r\n";

		//PASS
		constexpr const char *PASS_BFR_USER = "503 Login with user first.\r\n";
		constexpr const char *LOGIN_SUCCESS = "230 Login successful.\r\n";
		constexpr const char *LOGIN_FAIL = "530 Login incorrect.\r\n";

		//PORT
    	constexpr const char *PORT_SUCC = "200 PORT command successful. Consider using PASV instead.\r\n";
    	constexpr const char *INVALID_ARGS = "501 Syntax error in parameters or arguments.\r\n";
    	constexpr const char *GOODBYE = "221 Goodbye, see you next time.\r\n";

		//PWD
    	constexpr const char *PWD_START = "257 \"";
		constexpr const char *PWD_END = "\" is the current directory.\r\n";

		//CWD
		constexpr const char *CWD_SUCCESS = "250 Directory successfully changed.\r\n";
		constexpr const char *CWD_NOTFOUND = "550 Failed to change directory, invalid path.\r\n";
		constexpr const char *CWD_PERM = "550 Access denied Cannot leave home directory.\r\n";

		//LIST
    	constexpr const char *LIST_SEND = "150 Here comes the directory listing.\r\n";

		//DELE
    	constexpr const char *DELE_COMPLETE = "250 Requested action completed.\r\n";
    	constexpr const char *DELE_FAIL = "550 Requested action not taken.\r\n";
		
		//NOOP
		constexpr const char *NOOP = "200 Command okay.\r\n";
	}
}

#endif // CONFIG_HPP