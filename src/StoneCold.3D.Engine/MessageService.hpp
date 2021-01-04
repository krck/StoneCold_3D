
#ifndef STONECOLD_EVENTMANAGER_H
#define STONECOLD_EVENTMANAGER_H

#include "Types.hpp"

namespace StoneCold::Engine {

	class Message {};

	//
	// MessageService Singleton
	// 
	class MessageService {
	private:
		// Private default constructor to force the use of GetInstance()
		MessageService() = default;

	public:
		// Delete copy ctr and copy assignment
		MessageService(const MessageService&) = delete;
		MessageService& operator=(const MessageService&) = delete;

		static MessageService* GetInstance() {
			// Static variable in fucntion will be instanciated on  
			// first function call and destroyed on programm exit
			static MessageService instance;
			return &instance;
		}
	};
}

#endif
