# BloDoc

BloDoc is a static one page frontend Markdown blog generator engine. I use it for documentation.
It use Showdown, Bulma and FontAwesome from CDN.

## How to use

Your folders/files have to follow this guideline for the script to work.
See examples folder for a concrete example.

Compile the generator (use c++17 flag for compilation) or use the precompiled binary for your platform.

Copy mainPageStyle.html in the src parent's folder and rename it indexStyle.html

Then run the generator in the src parent's folder to generate the index.html.

```

    appFolder
        |
        |-- src
        |   |-- MyCategory1
        |   |   |
        |   |   |-- MyPage.md
        |   |   |-- AnotherPage.md
        |   |
        |   |-- MyCategory2
        |       |
        |       |-- ThisArticle.md
        |
        |-- blodoc_generator.exe
        |
        |-- indexStyle.html
        |
        |-- index.html (will be generated)

```

You can use github pages to host everything as done for this repo page.

## Plans for the futur

My goal is to scale it to full remote blog/documentation application with on the fly editing (without the need to compile the index.html).
