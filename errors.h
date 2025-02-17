//
// Created by saphal69 on 2/17/25.
//

#ifndef ERRORS_H
#define ERRORS_H

enum class PingerResult {
    OK,
    ERR_CONFIGURATION_FILE_NOT_FOUND,
    ERR_CONFIGURATION_FILE_NOT_READABLE,
    ERR_CONFIGURATION_FILE_NOT_PARSEABLE,
};

#endif //ERRORS_H
