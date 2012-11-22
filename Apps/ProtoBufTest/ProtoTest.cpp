// ProtoTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "examples/addressbook.pb.h"

extern int Test_AddressBook();

int main()
{
    printf("ProtoBuf Test Start ...\n");

    // Verify that the version of the library that we linked against is
    // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    if (0 == Test_AddressBook())
    {
        printf("Test_AddressBook: testing failed!\n");
    }

    // Optional:  Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();

    printf("ProtoBuf Test End ...\n");
    return 0;
}
