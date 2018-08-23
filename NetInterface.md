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
    "cmd":"JoinGame",
    "arg":{
       "Name":"User Name",//1
       "MapID":"map",//2
       "GameType":"team",//3
       "TankType":"naive"//4
    }
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
            {
                "ID":"P1",
                "TankType":"naive",
                "localAddr":"192.168.1.2-3000",
                "NAT":"192.168.1.3-4000"
            },
            {"ID":"P2","TankType":"naive"},
            {"ID":"P3","TankType":"naive"}
        ]
    }
```
---