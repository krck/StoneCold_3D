
#ifndef STONECOLD_EVENTMANAGER_H
#define STONECOLD_EVENTMANAGER_H

#include "SDL_Base.hpp"
#include "Types.hpp"

namespace StoneCold::Engine {

//
// Event Manager (Singelton)
//
// Handling SDL_UserEvent registration and publishing
// (Header only because of all the important comments)
//
class EventManager {
public:
	const uint32 UserEventType;

	// Delete copy ctr and copy assignment
	EventManager(const EventManager&) = delete;
	EventManager& operator=(const EventManager&) = delete;

	static EventManager& GetInstance() {
		// Static variable in fucntion will be instanciated on  
		// first function call and destroyed on programm exit
		static EventManager instance;
		return instance;
	}

	//
	// Publish a SDL_UserEvent
	//
	// The Event-Type will be hardcoded to the one received by SDL_RegisterEvents
	// The Event-User-Code will be used to distinguish different SDL_UserEvents
	//
	// Return false? Cause of failure is most likley a already full SDL-Event-Queue 
	//
	bool PublishEvent(EventCode eventCode) { return CreateAndPushEvent(eventCode, 0, 0); }
	bool PublishEvent(EventCode eventCode, void* data) { return CreateAndPushEvent(eventCode, data, 0); }
	bool PublishEvent(EventCode eventCode, void* data1, void* data2) { return CreateAndPushEvent(eventCode, data1, data2); }

	~EventManager() = default;

private:
	// Private default ctr used to register a user-defined SDL Event
	EventManager() : UserEventType(SDL_RegisterEvents(1)) {
		// Check if Registration worked (Should never be a problem with only one User-Event)
		if (UserEventType != ((uint32)-1)) {
			// It is theoretically possible to register around 32k different User-Events (event.type)
			// but not necessary, because User-Events can also hold different codes (event.user.code)
			// Therefore: Register one User-Event-Type and distinguish by Event-Code
		}
	}

	bool CreateAndPushEvent(EventCode eventCode, void* data1, void* data2) {
		try {
			// Create the default SDL_UserEvent
			SDL_Event event;
			SDL_memset(&event, 0, sizeof(event));
			event.type = UserEventType;

			// Add the custom Event data
			event.user.code = eventCode;
			event.user.data1 = data1;
			event.user.data2 = data2;

			// Documentation SDL_PushEvent:
			// The event is copied into the queue, and the caller may dispose of the memory pointed to after SDL_PushEvent() returns
			// ... So its save to just publish the Event and return its success state, even tho the &event is no use after this.
			return (SDL_PushEvent(&event) >= 0);
		}
		catch (...) {
			return false;
		}
	}

};

}

#endif
