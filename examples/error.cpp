#include <file.hpp>
#include <xift.hpp>


int main()
{
    XiftFile exporter(stdout);
    Xift     xift(exporter);

    xift.permitted.Tag("p").Attribute("class");

    xift.PutFile("error.html");

    if (!xift.End()) {
        printf("Error on %d line, %d column:\n%s",
               xift.ErrorLine(), xift.ErrorColumn(), xift.Error());
    }

    return 0;
}
