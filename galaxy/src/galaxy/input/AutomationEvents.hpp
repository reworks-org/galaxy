//// Automation events functionality
// AutomationEventList LoadAutomationEventList(const char* fileName);       // Load automation events list from file, NULL for empty list, capacity = MAX_AUTOMATION_EVENTS
// void                UnloadAutomationEventList(AutomationEventList list); // Unload automation events list from file
// bool                ExportAutomationEventList(AutomationEventList list, const char* fileName); // Export automation events list as text file
// void                SetAutomationEventList(AutomationEventList* list);                         // Set automation event list to record to
// void                SetAutomationEventBaseFrame(int frame);                                    // Set automation event internal base frame to start recording
// void                StartAutomationEventRecording(void);                                       // Start recording automation events (AutomationEventList must be set)
// void                StopAutomationEventRecording(void);                                        // Stop recording automation events
// void                PlayAutomationEvent(AutomationEvent event);                                // Play a recorded automation event
