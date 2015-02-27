package com.balsamiq.EventBusObserver;

/**
 * Created by morsini on 26/02/15.
 */
public class EventsPerSecondEvent {
    
    int events;

    public int getEvents() {
        return events;
    }

    public EventsPerSecondEvent(int numEvents)
    {
        events = numEvents;

    }
}
