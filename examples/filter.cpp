#include <file.hpp>
#include <xift.hpp>


int main()
{
    XiftFile exporter(stdout);
    Xift     xift(exporter);

    xift.Tag("a").Attribute("href");
    XiftTag &p = xift.Tag("p");
    p.Attribute("class");
    p.Attribute("hidden");
    p.Attribute("id");
    xift.Tag("script");

    xift.Remove("script");
    p.Remove("id");

    xift.PutFile("message.html");

    if (!xift.End()) {
        printf("Error on %d line, %d column:\n%s",
               xift.ErrorLine(), xift.ErrorColumn(),
               xift_error_message(xift.GetError()));
    }

    return 0;
}
