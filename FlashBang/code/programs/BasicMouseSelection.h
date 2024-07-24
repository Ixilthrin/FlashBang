#pragma once

#include "AppContext.h"

#include <memory>

using std::shared_ptr;

class BasicMouseSelection
{
public:
    BasicMouseSelection();
    virtual ~BasicMouseSelection();
    int Start();
private:
    std::shared_ptr<AppContext> _context;
};

