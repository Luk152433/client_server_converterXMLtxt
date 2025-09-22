#include "header.h"
#include <gtest/gtest.h>
#include <pugixml.hpp>

// Test konwersji z TXT do XML
TEST(FileConverterTest, TxtToXmlTest) {
    std::string txtContent = "w1 w2\nw3 w4";
    std::string expectedXml = R"(<root>
  <row>
    <col>w1</col>
    <col>w2</col>
  </row>
  <row>
    <col>w3</col>
    <col>w4</col>
  </row>
</root>)";

    std::string xmlResult = conv::FileConverter().txtToXml(txtContent);


    size_t pos = xmlResult.find("<?xml");
    if (pos != std::string::npos) {
        size_t endPos = xmlResult.find("?>", pos);
        if (endPos != std::string::npos) {
            xmlResult = xmlResult.substr(endPos + 2);
        }
    }
 
    pos = expectedXml.find("<?xml");
    if (pos != std::string::npos) {
        size_t endPos = expectedXml.find("?>", pos);
        if (endPos != std::string::npos) {
        expectedXml = expectedXml.substr(endPos + 2);
        }
    }

    pugi::xml_document doc1, doc2;
    pugi::xml_parse_result result1 = doc1.load_string(expectedXml.c_str(), pugi::parse_trim_pcdata);
    pugi::xml_parse_result result2 = doc2.load_string(xmlResult.c_str(), pugi::parse_trim_pcdata);
    
    ASSERT_TRUE(result1) << " parsing failed";
    ASSERT_TRUE(result2) << " parsing failed";

    EXPECT_TRUE(doc1 == doc2) << "TXT to XML conversion failed";
}

// Test konwersji z XML do TXT
TEST(FileConverterTest, XmlToTxtTest) {
    std::string xmlContent = R"(
  <row>
    <col>w1</col>
    <col>w2</col>
  </row>
  <row>
    <col>w3</col>
    <col>w4</col>
  </row>)";
    std::string expectedTxt = "w1 w2\nw3 w4\n";

    std::string txtResult = conv::FileConverter().xmlToTxt(xmlContent);
    EXPECT_EQ(txtResult, expectedTxt) << "XML to TXT conversion failed.";
}
