
#ifndef STONECOLD_EXCEPTION_H
#define STONECOLD_EXCEPTION_H

#include <exception>
#include <sstream>
#include <string>

namespace StoneCold::Base {

	class Exception : public std::exception {
	public:
		Exception(int line, const char* file, std::string message) noexcept;

		const char* what() const noexcept override;
		inline int GetLine() const noexcept { return _line; }
		inline std::string GetFile() const noexcept { return _file; }

	private:
		const std::string _errorMessage;
		const int _line;
		const std::string _file;
	protected:
		mutable std::string _whatBuffer;
	};

	// Exception macro, to add LINE and FILE automatically
	#define GameException(message) Exception(__LINE__, __FILE__, message)

}

#endif
