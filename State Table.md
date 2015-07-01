# State Table

State table contains a list of states, a description, the inputs and the next
state.

Each state can be one of four types.

| Type | Description |
| :--- | :---------- |
| MSG  | Display message |
| INPUT | Process input |
| WAIT  | Specific input type awaiting a specific key |
| ACTION | Do something |

INPUT and WAIT states typically require a MSG state prior to entry into these
state types.

ACTION states typically

| #   | Description                                            | Input | Next  |
| :-- | :----------------------------------------------------- | :---- | :---- |
| u   | default                                                |       | 0     |
| 0   | MSG:Select Mode                                        |       | 1     |
| 1   | INPUT:Mode                                             | D     | 2     |
|     |                                                        | A     | 8     |
| 2   | MSG:Division Entry                                     |       | 3     |
| 3   | INPUT:Number of divs                                   |       |       |
|     |   --> Expects 2 or greater                             | #<2   | 2     |
|     |                                                        | >=2   | 4     |
| 4   | MSG:Run Cutter                                         |       |       |
|     |   --> if(division == totalDivisions)                   | CS    | 7     |
|     |                                                        | CS    | 5     |
| 5   | WAIT:CS to rotate                                      | CS    | 6     |
| 6   | ACTION:rotate                                          |       | 4     |
| 7   | WAIT:CS for final pass                                 | CS    | 0     |
| 8   | MSG:Angle Entry                                        |       | 9     |
| 9   | INPUT:Angle                                            |       |       |
|     |   --> if(angle == 0) go to main menu                   | #=0   | 0     |
|     |   --> if(angle > 0 )                                   | #>0   | 10    |
| 10  | ACTION:rotate n degrees                                |       | 9     |
