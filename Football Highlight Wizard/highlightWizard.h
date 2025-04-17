#ifndef HIGHLIGHTWIZARD_H
#define HIGHLIGHTWIZARD_H

#include <vector>
#include <string>
#include <stdexcept> // For exceptions

// Enum to represent different types of game events
enum class EventType {
    GOAL,
    YELLOW_CARD,
    RED_CARD,
    FOUL,
    SUBSTITUTION,
    GAME_START,
    HALF_TIME,
    FULL_TIME,
    OTHER, // For events not explicitly handled
    UNKNOWN // For parsing errors
};

// Struct to hold information about a single game event
struct GameEvent {
    int timestampMinutes = -1;
    EventType type = EventType::UNKNOWN;
    std::string description;

    // Optional: Add player, team info later if needed
    // std::string player;
    // std::string team;

    // Helper to convert EventType to string for printing
    static std::string typeToString(EventType t);
    // Helper to convert string to EventType for parsing
    static EventType stringToType(const std::string& s);
};

// Exception class for file parsing errors
class ParseException : public std::runtime_error {
public:
    ParseException(const std::string& message) : std::runtime_error(message) {}
};


// The main class responsible for processing events and finding highlights
class HighlightWizard {
public:
    HighlightWizard() = default; // Use default constructor

    // Loads events from a specified file
    // Throws std::runtime_error if file cannot be opened
    // Throws ParseException if a line has incorrect format
    bool loadEventsFromFile(const std::string& filename);

    // Analyzes the loaded events and identifies highlights
    void findHighlights();

    // Returns a constant reference to the vector of identified highlights
    const std::vector<GameEvent>& getHighlights() const;

    // Prints the identified highlights to the console
    void printHighlights() const;

    // Returns a constant reference to the vector of aLL events
    const std::vector<GameEvent>& getAllEvents() const; 

private:
    std::vector<GameEvent> allEvents;       // Stores all events loaded from the file
    std::vector<GameEvent> highlightEvents; // Stores only the events identified as highlights

    // Internal helper to parse a single line from the event file
    GameEvent parseEventLine(const std::string& line, int lineNumber);

    // Internal helper to determine if a given event qualifies as a highlight
    bool isHighlight(const GameEvent& event) const;
};

#endif // HIGHLIGHTWIZARD_H