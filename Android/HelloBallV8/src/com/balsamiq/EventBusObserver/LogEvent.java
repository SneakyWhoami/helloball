package com.balsamiq.EventBusObserver;

/**
 * Created by morsini on 26/02/15.
 */
public class LogEvent {
    
    private String message;

    public String getMessage() {
        return message;
    }

    public LogEvent(String logMessage)
    {
        message = logMessage;

    }
}
