#ifndef CONVERTER_H
#define CONVERTER_H


namespace conv{
    class FileConverter {
    public:

    std::string txtToXml(const std::string &fileContent);
    std::string xmlToTxt(const std::string &fileContent);
    
    };
}

#endif
