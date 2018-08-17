Unity to LocalServer:
===
Commands:
 - CAU: local input
 ```json
 {
    "ID":"addrID",
    "CMDS":
    [
        {"v1":100,"v1":100},
        {"v1":200,"v1":200},
        {"v1":300,"v1":300}
    ]
}
 ```
 - RES: local output
 ```json
{
    "dat":
    [
        {
            "id":"addrID0",
            "dats":
            [
                {},
                {},
                {}
            ]
        },
        {
            "id":"addrID1",
            "dats":
            [
                {},
                {},
                {}
            ]
        }
    ]
}
 ```
 - OPR: local instruction
 ```json
 {
     "cmd":"joinGame",
 }
 ```
 ---

 LocalServer to Unity:
 ===
  - CAU: packed inputs
```json
{
    "frm":0,
    "msg":
    [
        {
            "ID":"addrID0",
            "CMDS": 
            [
                {"v1":100,"v1":100},
                {"v1":200,"v1":200},
                {"v1":300,"v1":300}
            ]
        },
        {
            "ID":"addrID1",
            "CMDS": 
            [
                {"v1":100,"v1":100},
                {"v1":200,"v1":200},
                {"v1":300,"v1":300}
            ]
        }
    ]
}
```
  - INI: init param
```json
    {
        "locID": "addrID",
        "members:
        [
            "P1",
            "P2",
            "P3"
        ]
    }
```
---



