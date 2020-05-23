/*
Copyright (c) 2020 Floren Vain

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;



const std::string CATEGORIES_LINK_INSERT_TAG_STRING = "[INSERT_CATEGORIES_LINK_HERE]";
//category name (created from directory name where underscore is space)
const std::string CATEGORY_NAME_INSERT_TAG_STRING = "[INSERT_CATEGORY_NAME_HERE]";

const std::string CATEGORY_PANELS_INSERT_TAG_STRING = "[INSERT_CATEGORY_PANELS_HERE]";
const std::string CATEGORY_PANEL_ARTCLES_LINK_TAG_STRING = "[INSERT_CATEGORY_PANEL_ARTICLES_LINK_HERE]";

//article name (created from article filename where underscore is space)
const std::string ARTICLE_NAME_INSERT_TAG_STRING = "[INSERT_ARTICLE_NAME_HERE]";
//article path from src folder
const std::string ARTICLE_PATH_INSERT_TAG_STRING = "[INSERT_ARTICLE_PATH_HERE]";

const std::string categoryMenuLinkHTMLTemplate_firstOne = R"(<li><a class="is-active is-link">)" + CATEGORY_NAME_INSERT_TAG_STRING + "</a></li>";
const std::string categoryMenuLinkHTMLTemplate = "<li><a>" + CATEGORY_NAME_INSERT_TAG_STRING + "</a></li>";

const std::string categoryPanelHTMLTemplate_firstOne = R"(
<nav class = "panel is-warning" style = "box-shadow:none; border-radius: 0; border-left: solid 1px rgba(0, 0, 0, 0.1); height: 100%;">
    <div style = "overflow-y: scroll; height: 100%; scrollbar-width: thin;">)" + CATEGORY_PANEL_ARTCLES_LINK_TAG_STRING + "</div></nav>";

const std::string categoryPanelHTMLTemplate = R"(
<nav class = "panel is-warning is-hidden" style = "box-shadow:none; border-radius: 0; border-left: solid 1px rgba(0, 0, 0, 0.1); height: 100%;">
    <div style = "overflow-y: scroll; height: 100%; scrollbar-width: thin;">)" + CATEGORY_PANEL_ARTCLES_LINK_TAG_STRING + "</div></nav>";


const std::string articleHTMLTemplate_firstOne = R"(
<a class="panel-block is-active has-background-link has-text-light" onclick="loadPage(this);" link=")" + ARTICLE_PATH_INSERT_TAG_STRING + R"(">
    <span class = "panel-icon">
    <i class="fas fa-book" aria-hidden="true"></i>
    </span>)" + ARTICLE_NAME_INSERT_TAG_STRING + "</a>";

const std::string articleHTMLTemplate = R"(
<a class="panel-block" onclick="loadPage(this);" link=")" + ARTICLE_PATH_INSERT_TAG_STRING + R"(">
    <span class="panel-icon">
    <i class="fas fa-book" aria-hidden="true"></i>
    </span>)" + ARTICLE_NAME_INSERT_TAG_STRING + "</a>";


void ScanDir(std::vector<fs::directory_entry>& paths, fs::path pathToDirToScan)
{
    for (const auto& entry : fs::directory_iterator(pathToDirToScan))
        paths.push_back(entry);
}

void ListDirectories(std::vector<fs::directory_entry>& entries)
{
    ScanDir(entries, "src");

    entries.erase(std::remove_if(entries.begin(), entries.end(),
        [](fs::directory_entry& entry) { return !entry.is_directory(); }), entries.end());

}

void ListFiles(std::vector<fs::directory_entry>& entries, fs::path path)
{
    ScanDir(entries, path);

    entries.erase(std::remove_if(entries.begin(), entries.end(),
        [](fs::directory_entry& entry) { return !entry.is_regular_file(); }), entries.end());
}

std::string getFileName(fs::path path)
{
    std::string filename = path.filename().string();

    return filename.substr(0, filename.find('.'));
}

void replace(std::string& workingString, const std::string tag, std::string replaceValue)
{
    workingString.replace(workingString.find(tag), tag.size(), replaceValue);
}

void replaceF(std::string& workingString, const std::string tag, std::string &replaceValue)
{
    workingString.replace(workingString.find(tag), tag.size(), replaceValue);
}


int main()
{
    std::cout << "BloDoc generator\n";

    if (std::filesystem::exists("src"))
    {
        std::vector<std::filesystem::directory_entry> entries;
        ListDirectories(entries);


        std::string htmlBase = "";
        std::fstream file("indexStyle.html", std::ios::in | std::ios::ate);

        size_t fileSize = (size_t)file.tellg();
        htmlBase.resize(fileSize);

        file.seekg(0);
        file.read(htmlBase.data(), fileSize);
        file.close();


        std::vector<std::filesystem::directory_entry> articles;
        std::string workString = "";
        std::string workString_bis = "";

        std::string categoryPanels = "";
        std::string categoryLinks = "";
        bool isFirstCategory = true;

        for (auto& entry : entries)
        {
            std::cout << entry.path() << std::endl;

            articles.clear();
            ListFiles(articles, entry.path());
            std::string articlesHTMLLinkList = "";

            bool isFirstArticle = true;

            for (auto& article : articles)
            {
                if (isFirstArticle)
                {
                    workString = articleHTMLTemplate_firstOne;
                    isFirstArticle = false;
                }
                else
                {
                    workString = articleHTMLTemplate;
                }

                replace(workString, ARTICLE_PATH_INSERT_TAG_STRING, article.path().string());
                replace(workString, ARTICLE_NAME_INSERT_TAG_STRING, getFileName(article.path()));
                
                articlesHTMLLinkList += workString + "\n";
            }

            if (isFirstCategory)
            {
                workString = categoryPanelHTMLTemplate_firstOne;
                workString_bis = categoryMenuLinkHTMLTemplate_firstOne;
                isFirstCategory = false;
            }
            else
            {
                workString = categoryPanelHTMLTemplate;
                workString_bis = categoryMenuLinkHTMLTemplate;
            }


            replace(workString, CATEGORY_PANEL_ARTCLES_LINK_TAG_STRING, articlesHTMLLinkList);

            categoryPanels += workString + "\n";

            replace(workString_bis, CATEGORY_NAME_INSERT_TAG_STRING, entry.path().filename().string());

            categoryLinks += workString_bis + "\n";

        }

        replace(htmlBase, CATEGORY_PANELS_INSERT_TAG_STRING, categoryPanels);
        replace(htmlBase, CATEGORIES_LINK_INSERT_TAG_STRING, categoryLinks);

        std::ofstream ofile("index.html", std::ios::trunc);
       
        ofile << htmlBase << std::endl;

        ofile.close();

    }
    else
    {
        std::cerr << "src folder does not exist \n";
    }

    std::string a = "";
    std::cout << "BLODOC generation ended. Type Y to close : ";
    std::cin >> a;
    
}


 
