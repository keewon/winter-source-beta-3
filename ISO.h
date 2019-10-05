#ifndef _ISO_h_
#define _ISO_h_

// ISO/IEC 14882-1998 Information Technology - The Programming Language - C++
#define for if (true) for
#pragma warning(disable: 4786)

namespace ISO
{
    class Program;
};

class ISO::Program
{
public:
    virtual int main(void) = 0;

    virtual ~Program(void) { }
};

#endif//_ISO_h_