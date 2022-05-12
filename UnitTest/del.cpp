namespace FOO {

class ABC {};
class XYZ {};

template <class T, class U> class TemplateType {};

}

using namespace FOO;



int main() {
    TemplateType<ABC, XYZ> Decl;
    FOO::TemplateType<ABC, XYZ> Decl2;
    return 0;
}