#include "highlightWizard.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm> // For std::find_if, std::transform etc. if needed later
#include <stdexcept> // For runtime_error

// --- GameEvent Static Helper Implementations ---

std::string GameEvent::typeToString(EventType t) {
    switch (t) {
        case EventType::GOAL:         return "GOAL";
        case EventType::YELLOW_CARD:  return "YELLOW_CARD";
        case EventType::RED_CARD:     return "RED_CARD";
        case EventType::FOUL:         return "FOUL";
        case EventType::SUBSTITUTION: return "SUBSTITUTION";
        case EventType::GAME_START:   return "GAME_START";
        case EventType::HALF_TIME:    return "HALF_TIME";
        case EventType::FULL_TIME:    return "FULL_TIME";
        case EventType::OTHER:        return "OTHER";
        case EventType::UNKNOWN:      return "UNKNOWN";
        default:                      return "INVALID_TYPE";
    }
}

EventType GameEvent::stringToType(const std::string& s) {
    std::string upper_s = s;
    // Convert to uppercase for case-insensitive comparison
    std::transform(upper_s.begin(), upper_s.end(), upper_s.begin(), ::toupper);

    if (upper_s == "GOAL") return EventType::GOAL;
    if (upper_s == "YELLOW_CARD") return EventType::YELLOW_CARD;
    if (upper_s == "RED_CARD") return EventType::RED_CARD;
    if (upper_s == "FOUL") return EventType::FOUL;
    if (upper_s == "SUBSTITUTION") return EventType::SUBSTITUTION;
    if (upper_s == "GAME_START") return EventType::GAME_START;
    if (upper_s == "HALF_TIME") return EventType::HALF_TIME;
    if (upper_s == "FULL_TIME") return EventType::FULL_TIME;
    if (upper_s == "OTHER") return EventType::OTHER;

    return EventType::UNKNOWN; // Return UNKNOWN if string doesn't match
}


// --- HighlightWizard Member Function Implementations ---

bool HighlightWizard::loadEventsFromFile(const std::string& filename) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        // It's often better to throw an exception here or return false and let the caller handle it
        // std::cerr << "Error: Could not open file " << filename << std::endl;
         throw std::runtime_error("Could not open file: " + filename);
        // return false;
    }

    allEvents.clear(); // Clear previous events
    highlightEvents.clear(); // Also clear previous highlights

    std::string line;
    int lineNumber = 0;
    while (std::getline(inputFile, line)) {
        lineNumber++;
        // Skip empty lines or potential comment lines (e.g., starting with '#')
        if (line.empty() || line[0] == '#') {
            continue;
        }

        try {
            allEvents.push_back(parseEventLine(line, lineNumber));
        } catch (const ParseException& e) {
            // Propagate the specific parse error
            inputFile.close(); // Ensure file is closed before throwing
            throw ParseException(std::string(e.what()) + " in file " + filename);
        } catch (const std::exception& e) {
            // Catch other potential errors during parsing (e.g., std::stoi)
             inputFile.close();
             throw ParseException("Error parsing line " + std::to_string(lineNumber) + ": " + e.what() + " in file " + filename);
        }
    }

    inputFile.close();
    return true;
}

GameEvent HighlightWizard::parseEventLine(const std::string& line, int lineNumber) {
    std::stringstream ss(line);
    std::string segment;
    std::vector<std::string> parts;

    while (std::getline(ss, segment, ',')) { // Assuming CSV format
        parts.push_back(segment);
    }

    if (parts.size() < 3) {
        throw ParseException("Error parsing line " + std::to_string(lineNumber) + ": Expected at least 3 comma-separated values, found " + std::to_string(parts.size()) + ". Line: '" + line + "'");
    }

    GameEvent event;
    try {
        // Part 0: Timestamp
        event.timestampMinutes = std::stoi(parts[0]);
        if (event.timestampMinutes < 0) {
             throw std::out_of_range("Timestamp cannot be negative.");
        }

        // Part 1: Event Type
        event.type = GameEvent::stringToType(parts[1]);
         if(event.type == EventType::UNKNOWN) {
             std::cerr << "Warning line " << lineNumber << ": Unknown event type '" << parts[1] << "'. Treating as OTHER." << std::endl;
             // Optionally treat unknown as OTHER or throw an error depending on strictness
              event.type = EventType::OTHER;
         }


        // Part 2: Description (remainder of the line, could contain commas)
        // Reconstruct description if it contained commas
        event.description = parts[2];
        for(size_t i = 3; i < parts.size(); ++i) {
            event.description += "," + parts[i];
        }
        // Trim leading/trailing whitespace from description (optional but good)
         size_t first = event.description.find_first_not_of(' ');
         if (std::string::npos == first) {
             event.description = ""; // Handle empty or all-whitespace description
         } else {
             size_t last = event.description.find_last_not_of(' ');
             event.description = event.description.substr(first, (last - first + 1));
         }


    } catch (const std::invalid_argument& ia) {
        throw ParseException("Error parsing line " + std::to_string(lineNumber) + ": Invalid number format for timestamp. Value: '" + parts[0] + "'");
    } catch (const std::out_of_range& oor) {
        throw ParseException("Error parsing line " + std::to_string(lineNumber) + ": Timestamp number out of range. Value: '" + parts[0] + "'");
    }

    return event;
}

bool HighlightWizard::isHighlight(const GameEvent& event) const {
    // Define which event types are considered highlights
    switch (event.type) {
        case EventType::GOAL:
        case EventType::RED_CARD:
        // Potentially add others later, e.g.:
        // case EventType::PENALTY_SCORED:
        // case EventType::GREAT_SAVE: // (Would require more complex input/logic)
            return true;
        default:
            return false;
    }
}

void HighlightWizard::findHighlights() {
    highlightEvents.clear(); // Ensure we start fresh
    for (const auto& event : allEvents) {
        if (isHighlight(event)) {
            highlightEvents.push_back(event);
        }
    }
    // Optional: Sort highlights by timestamp if loading order wasn't guaranteed
    // std::sort(highlightEvents.begin(), highlightEvents.end(),
    //           [](const GameEvent& a, const GameEvent& b) {
    //               return a.timestampMinutes < b.timestampMinutes;
    //           });
}

const std::vector<GameEvent>& HighlightWizard::getHighlights() const {
    return highlightEvents;
}

const std::vector<GameEvent>& HighlightWizard::getAllEvents() const {
    return allEvents;}

void HighlightWizard::printHighlights() const {
    if (highlightEvents.empty()) {
        std::cout << "No highlights found in the loaded events." << std::endl;
        return;
    }

    std::cout << "\n--- Football Highlights ---" << std::endl;
    for (const auto& event : highlightEvents) {
        std::cout << "[" << event.timestampMinutes << "'] "
                  << GameEvent::typeToString(event.type) << ": "
                  << event.description << std::endl;
    }
    std::cout << "-------------------------" << std::endl;
}