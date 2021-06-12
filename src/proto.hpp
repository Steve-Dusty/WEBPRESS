#include <regex>
#include <unordered_map>

using namespace std;

unordered_map<string, string> httpOut;

// REGULAR EXPRESSIONS //
regex startLine("^(GET|HEAD|POST|PUT|DELETE|CONNECT|OPTIONS|TRACE|PATCH) (/[A-z].*) HTTP/1.1");
regex headers("(.*): (.*)");

smatch match;
// REGUALR EXPRESSIONS //

bool parse(char request[]) {
    string req = request;
    httpOut.clear();

    if (regex_search(req, match, startLine)) {
        // SET THE PRESETS OF IT //
        httpOut["method"] = match.str(1);
        httpOut["path"] = match.str(2);
        // SET THE PRESETS OF IT //
    } else {
        return false;
    }


    return true;
}
