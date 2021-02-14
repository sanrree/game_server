#pragma once

enum class Request
{
    JOIN_GAME = 48,
    END_GAME = 49,
    SEND_DATA = 50,
};

enum class Response
{
    WAIT_FOR_PLAYER,
    START_GAME,
    END_GAME,
    GET_DATA,
};