#include <file.hpp>
#include <xift.hpp>


int main()
{
    XiftFile exporter(stdout);
    Xift     xift(exporter);

    xift.Tag("p").Attribute("class");

    xift.PutFile("error.html");

    if (!xift.End()) {
        printf("Error on %d line, %d column:\n%s",
               xift.ErrorLine(), xift.ErrorColumn(),
               xift_error_message(xift.GetError()));
    }

    return 0;
}
