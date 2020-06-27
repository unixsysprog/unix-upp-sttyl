/**
 * sttyltables.c - defines the flags values and names, that is available in the
 * light weight version of stty.
 * date: 3/2/2017
 * author: Tasuku Miura
 */
#include "sttyltables.h"


const struct flaginfo local_modes[] = {
    { IEXTEN    , "iexten"},
    { ECHO      , "echo"},
    { ECHOE     , "echoe" },
    { ECHOK     , "echok"},
    { ICANON    , "icanon"},
    { ISIG      , "isig"},
    { -1        , NULL}
};

const struct flaginfo input_modes[] = {
    { BRKINT    , "brkint"},
    { INPCK     , "inpnk"},
    { ICRNL     , "icrnl" },
    { IXANY     , "ixany"},
    { -1        , NULL}
};

const struct flaginfo output_modes[] = {
    { OPOST    , "opost"},
    { ONLCR     , "onlcr"},
    { OCRNL     , "ocrnl" },
    { -1        , NULL}
};

const struct flaginfo control_modes[] = {
    { PARENB    , "parenb"},
    { HUPCL     , "hupcl"},
    { CREAD     , "cread"},
    { -1        , NULL}
};

const struct flaginfo special_chars[] = {
    { VINTR    , "intr"},
    { VERASE   , "erase"},
    { VKILL    , "kill"},
    { VSTART   , "start"},
    { VSTOP    , "stop"},
    { VWERASE  , "werase"},
    { -1       , NULL}
};
