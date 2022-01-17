#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int NO_OF_TEAMS;
int TOTAL_MATCHES;
vector<pair<int, int>> totalCombinations;
vector<vector<int>> computingResultIndexStack; // Using the outer vector as a stack

void printComputationalResultIndexStack()
{
    int i = 0;
    for (auto pVector : computingResultIndexStack)
    {
        cout << "\nMatchDay" << ++i << " : ";
        for (auto item : pVector)
        {
            pair<int, int> p = totalCombinations[item];
            cout << "{" << p.first << "," << p.second << "}" << " ";
        }
    }
    cout << "\nTotal MatchDays : " << computingResultIndexStack.size() << endl;
}

void InitTotalCombinations(int n)
{
    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= n; ++j)
        {
            if (i == j) continue;
            totalCombinations.push_back({i, j});
        }
    }
}

void ComputeTeams(vector<int> selectedMatchDayTeams = vector<int>(), vector<int> alreadyMatched = vector<int>(), int startIndex = 0)
{
    for (int i = startIndex; i < TOTAL_MATCHES; i++)
    {
        int teamNumber = (i / (NO_OF_TEAMS - 1)) + 1;
        
        // Is Team already scheduled to play today?
        if (any_of(selectedMatchDayTeams.begin(), selectedMatchDayTeams.end(), [&](int idx)
                    { return totalCombinations[idx].first == teamNumber || totalCombinations[idx].second == teamNumber; }))
        {
            i = (teamNumber * (NO_OF_TEAMS -1)) - 1; //teamNumberEndIndex
            continue;
        }
        
        // Is same match already scheduled in the tournament?
        if (find(alreadyMatched.begin(), alreadyMatched.end(), i) != alreadyMatched.end())
        {
            continue;
        }
        
        // Take a match pick
        pair<int, int> tempMatchPick = totalCombinations[i];
        
        // Is the "other team" selected - already scheduled to play today?
        if (any_of(selectedMatchDayTeams.begin(), selectedMatchDayTeams.end(), [&](int idx)
                    { return totalCombinations[idx].first == tempMatchPick.second || totalCombinations[idx].second == tempMatchPick.second; }))
        {
            continue;
        }
        
        // Validate the pick with previous day matches
        if (int count = computingResultIndexStack.size(); count > 0)
        {
            vector<int> previousMatchDayIndexes = computingResultIndexStack[count - 1]; // Last pushed item
            
            // Team which played "home" match previous day is tried to be scheduled for a home match again
            if (any_of(previousMatchDayIndexes.begin(), previousMatchDayIndexes.end(), [&](int idx)
                        { return totalCombinations[idx].first == tempMatchPick.first; }))
            {
                i = (teamNumber * (NO_OF_TEAMS -1)) - 1; //teamNumberEndIndex
                continue;
            }
            // Team which played "away" match previous day is tried to be scheduled for a away match again
            if (any_of(previousMatchDayIndexes.begin(), previousMatchDayIndexes.end(), [&](int idx)
                        { return totalCombinations[idx].second == tempMatchPick.second; }))
            {
                continue;
            }
        }
        selectedMatchDayTeams.push_back(i); // Add the tempMatchPick's index as picked match
    }
    
    if (selectedMatchDayTeams.size() == NO_OF_TEAMS / 2)
    {
        computingResultIndexStack.push_back(selectedMatchDayTeams);
        
        alreadyMatched.insert(alreadyMatched.end(), selectedMatchDayTeams.begin(), selectedMatchDayTeams.end());
        
        // Print the so far computed results
        if (computingResultIndexStack.size() >= NO_OF_TEAMS)
        {
            printComputationalResultIndexStack();
        }
        
        if (alreadyMatched.size() < TOTAL_MATCHES)
        {
            return ComputeTeams(vector<int>{}, alreadyMatched, 0);
        }
    }
    else
    {
        if (selectedMatchDayTeams.size() > 0)
        {
            int lastSuccessIndex = selectedMatchDayTeams.back();
            
            // BackTrack - Remove the last item from selectedMatchDayTeams
            selectedMatchDayTeams.pop_back();
            
            // Put the items into selectedMatchDayTeams and proceed
            return ComputeTeams(selectedMatchDayTeams, alreadyMatched, lastSuccessIndex + 1);
        }
        else
        {
            if (computingResultIndexStack.size() > 0)
            {
                // BackTrack - Remove the entire last match day entry from computingResultIndexStack now
                vector<int> lastComputedMatchDaySchedule = move(computingResultIndexStack.back()); 
                computingResultIndexStack.pop_back();
                
                // Remove the last day scheduled matches from alreadyMatched
                alreadyMatched.erase(remove_if(alreadyMatched.begin(), alreadyMatched.end(), [&](int idx) {
                    return find(lastComputedMatchDaySchedule.begin(), 
                                lastComputedMatchDaySchedule.end(), idx) != lastComputedMatchDaySchedule.end();}),
                    alreadyMatched.end());
                
                // Remove the last match from the match day
                int lastSuccessIndex = lastComputedMatchDaySchedule.back();
                lastComputedMatchDaySchedule.pop_back();
                
                // Put the items into selectedMatchDayTeams and proceed
                return ComputeTeams(lastComputedMatchDaySchedule, alreadyMatched, lastSuccessIndex + 1);
            }
        }
    }
}

int main()
{
    cout << "Please enter the total number of teams(even number) to be scheduled matches:" << endl;
    cin >> NO_OF_TEAMS;
    if (NO_OF_TEAMS % 2 != 0)
    {
        cout <<"Sorry! Input should be in even number!" << endl;
        return -1;
    }
    TOTAL_MATCHES = (NO_OF_TEAMS * (NO_OF_TEAMS - 1));
    
    InitTotalCombinations(NO_OF_TEAMS);
    
    ComputeTeams();
    
    // Final schedule
    cout << "\n\n========================================" << endl;
    cout << "Final Feasible Schedule: ";
    if (computingResultIndexStack.size() > 0)
    {
        cout << "Given Below" << endl;
    }
    else
    {
        cout << "None! (Please see above for partial solutions afer which the constraints will be violated)" << endl;
    }
    cout << "========================================" << endl;
    printComputationalResultIndexStack();
    
    return 0;
}
