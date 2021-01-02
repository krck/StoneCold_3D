
#ifndef STONECOLD_KEYBOARD_H
#define STONECOLD_KEYBOARD_H

#include <queue>
#include <vector>

#include "Settings.hpp"
#include "Types.hpp"

namespace StoneCold::Game {

	//
	// KeyboardServer (Singelton)
	//
	// Global Keyboard Server that holds the current state of any Key and a buffer of all Key-presses in order.
	// This allows for quick "is key pressed?" checks as well as reading (looping) every recent keyboard input
	//
	class KeyboardServer {
	private:
		// Private default constructor to force the use of GetInstance()
		KeyboardServer() : _keyStates(std::vector<bool>(KEY_CODES, false)), _keyBuffer(std::queue<int>()) { }

	public:
		// Delete copy ctr and copy assignment
		KeyboardServer(const KeyboardServer&) = delete;
		KeyboardServer& operator=(const KeyboardServer&) = delete;

		static KeyboardServer* GetInstance() {
			// Static variable in fucntion will be instanciated on  
			// first function call and destroyed on programm exit
			static KeyboardServer instance;
			return &instance;
		}

		void OnKeyPressed(int keycode) {
			_keyStates[keycode] = true;
			_keyBuffer.push(keycode);
			// Keep only a number of key presses in the buffer
			if (_keyBuffer.size() == KEY_BUFFER_SIZE) {
				_keyBuffer.pop();
			}
		}

		inline void OnKeyReleased(int keycode) { _keyStates[keycode] = false; }
		inline void RemoveFirstKey() { _keyBuffer.pop(); }
		inline void FlushBuffer() { std::queue<int>().swap(_keyBuffer); }

		inline bool GetKeyState(int keycode) const { return _keyStates[keycode]; }
		inline size_t GetBufferSize() const { return _keyBuffer.size(); }
		inline int GetFirstKey() const { return _keyBuffer.front(); }

	private:
		std::vector<bool> _keyStates;
		std::queue<int> _keyBuffer;
	};

	//
	// KeyboardClient
	//
	// Each KeyboardClient is linked to the global KeyboardServer and can check
	// if any specific key was pressed or read all recent keyboard input in order
	//
	class KeyboardClient {
	public:
		KeyboardClient(KeyboardServer* kServer) : _keyboardServer(kServer) { }

		// Get constant Key presses
		inline bool IsKeyPressed(int keycode) const { return _keyboardServer->GetKeyState(keycode); }

		// Get pressed Keys one by one
		int ReadKey() const {
			if (_keyboardServer->GetBufferSize()) {
				const int keycode = _keyboardServer->GetFirstKey();
				_keyboardServer->RemoveFirstKey();
				return keycode;
			}
			return 0;
		}

		inline void FlushKeyBuffer() const { _keyboardServer->FlushBuffer(); }

	private:
		KeyboardServer* _keyboardServer;
	};

}

#endif
