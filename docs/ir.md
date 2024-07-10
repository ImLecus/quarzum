# Intermediate representation instruction guide

| type | args[0]        | args[1]     | args[2]    | args[3] | args[4] |
|------|----------------|-------------|------------|---------|---------|
| call | char* function | int tIndex  |            |         |         |
| add  | char* left     | char* right | char* dest |         |         |
| sub  | char* left     | char* right | char* dest |         |         | 
| mul  | char* left     | char* right | char* dest |         |         |
| div  | char* left     | char* right | char* dest |         |         |
| mod  | char* left     | char* right | char* dest |         |         |
|assign| char* value    | char* dest  |            |         |         |
| jmp  | char* location |             |            |         |         |