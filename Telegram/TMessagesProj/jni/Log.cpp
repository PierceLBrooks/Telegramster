
// Author: Pierce Brooks

#include "Log.hpp"

telegramster::Log* telegramster::Log::self = nullptr;

telegramster::Log::Log() :
    ss(new std::stringstream())
{
    self = this;
}

telegramster::Log::~Log()
{
    delete ss;
}

telegramster::Log* telegramster::Log::getSelf()
{
    if (self == nullptr)
    {
        new Log();
    }
    return self;
}
