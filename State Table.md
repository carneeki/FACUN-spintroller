# State Table
| #   | Description       | Input | Next  | Transition Description             |
| :-- | :---------------- | :---- | :---- | :--------------------------------- |
| 0   | Default State     |       |       |                                    |
|     |                   | D     | 1     | Enter division entry               |
|     |                   | A     | 6     | Enter angle entry                  |
| 1   | Division Entry    | #>=2  | 2     | MSG: Run cutter                    |
| 2   | MSG: Run Cutter   | CS    | 3     | Rotate divisions if more divisions to do      |
| 2   | MSG: Run Cutter   | CS    | 5     | Show finished if all divisions cut |
| 3   | Wait to next div  | CS    | 4     | Wait for CS to rotate to next division |
| 4   | Rotate work piece |       | 2     |                                    |
| 5   | MSG: Op Complete  | CS    | 0     | MSG: Op Complete                   |
| 6   | Enter Angle Entry | 0     | 0     | Default State                      |
|     | Angle Entry       | #>0   | 7     | MSG: Press CS                      |
| 7   | MSG: Press CS     | CS    | 6     | Enter Angle Entry                  |
