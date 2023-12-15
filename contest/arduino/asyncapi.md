# Slave API 0.1.0 documentation

## Table of Contents

In a packet, the first 5 bytes are the header and the later bytes are the data.

| Byte     | Description                                          |
| -------- | ---------------------------------------------------- |
| 0        | Always 0x55                                          |
| 1        | Always 0xAA                                          |
| 2 to 3   | Short. The number of bytes of the data (N)           |
| 4        | Byte. The checksum of the data                       |
| 5 to N+4 | The data of the packet. N is the length of the data. |

* [Operations](#operations)
  * [PUB main](#pub-main-operation)
  * [SUB main](#sub-main-operation)

## Operations

### PUB `main` Operation

#### Message `<anonymous-message-1>`

##### Payload

| Name       | Type    | Description                                                            | Value                   | Constraints     | Notes                                     |
| ---------- | ------- | ---------------------------------------------------------------------- | ----------------------- | --------------- | ----------------------------------------- |
| (root)     | object  | -                                                                      | -                       | -               | **additional properties are NOT allowed** |
| actionType | integer | The action type. 0: ATTACK 1: PLACE_BLOCK 2: TRADE                     | allowed (`0`, `1`, `2`) | format (`int8`) | -                                         |
| param      | integer | The parameter. Chunk id for ATTACK and PLACE_BLOCK. Item id for TRADE. | -                       | format (`int8`) | -                                         |

> Examples of payload _(generated)_

```json
{
  "actionType": 0,
  "param": 0
}
```

### SUB `main` Operation

#### Message `<anonymous-message-2>`

##### Payload

| Name                         | Type           | Description                                                | Value | Constraints      | Notes                                     |
| ---------------------------- | -------------- | ---------------------------------------------------------- | ----- | ---------------- | ----------------------------------------- |
| (root)                       | object         | -                                                          | -     | -                | **additional properties are NOT allowed** |
| 0gameStage                   | integer        | The game stage 0: READY 1: RUNNING 2: BATTLING 3: FINISHED | -     | format (`int8`)  | -                                         |
| 1elapsedTicks                | integer        | The elapsed ticks.                                         | -     | format (`int32`) | -                                         |
| 5heightOfChunks              | array<integer> | -                                                          | -     | 64 items         | -                                         |
| heightOfChunks (single item) | integer        | The height of chunks.                                      | -     | format (`int8`)  | -                                         |
| 69hasBed                     | boolean        | Whether the player has bed.                                | -     | -                | -                                         |
| 70hasBedOpponent             | boolean        | Whether the opponent has bed.                              | -     | -                | -                                         |
| 71positionX                  | number         | The x coordinate.                                          | -     | format (`float`) | -                                         |
| 75positionY                  | number         | The y coordinate.                                          | -     | format (`float`) | -                                         |
| 79positionOpponentX          | number         | The x coordinate of the opponent.                          | -     | format (`float`) | -                                         |
| 83positionOpponentY          | number         | The y coordinate of the opponent.                          | -     | format (`float`) | -                                         |
| 87agility                    | integer        | The agility point.                                         | -     | format (`int8`)  | -                                         |
| 88health                     | integer        | The health point.                                          | -     | format (`int8`)  | -                                         |
| 89maxHealth                  | integer        | The max health point.                                      | -     | format (`int8`)  | -                                         |
| 90strength                   | integer        | The strength point.                                        | -     | format (`int8`)  | -                                         |
| 91emeraldCount               | integer        | The emerald count.                                         | -     | format (`int8`)  | -                                         |
| 92woolCount                  | integer        | The wool count.                                            | -     | format (`int8`)  | -                                         |

> Examples of payload _(generated)_

```json
{
  "gameStage": 0,
  "elapsedTicks": 0,
  "heightOfChunks": [
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
  ],
  "hasBed": true,
  "hasBedOpponent": true,
  "positionX": 0,
  "positionY": 0,
  "positionOpponentX": 0,
  "positionOpponentY": 0,
  "agility": 0,
  "health": 0,
  "maxHealth": 0,
  "strength": 0,
  "emeraldCount": 0,
  "woolCount": 0
}
```
