#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>

const QString  SESSSION_KEY = "SCHEDULERKEEPER";

enum SortBy {
    SortBySaveTime,
    SortByCreatedTime,
    SortByRemainder,
    SortByNull,
};
#endif // GLOBAL_H
