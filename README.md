# FootballTournamentScheduling

OnlineGDB:
https://onlinegdb.com/CTXxy4_v9

Implemented a scheduling algorithm, which backtracks and tries to find a schedule 
within the given constraints. Following are constraints used:

The matches will be scheduled with the following manner
1. Each team should play each other team twice. Once as the home team and once as the away
team
2. Each team should have alternating home and away matches
3. All matches (I read it as "all team matches") are scheduled to be held on the same day
Example Input:
Teams are 1,2,3,4,5,6
Example Output
Matchday 1: 1,2. 3,4. 5,6
Matchday 2: 4,1. 6,3. 2,5.
Etc.
