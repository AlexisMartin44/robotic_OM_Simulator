#ifndef SVGFILE_H_INCLUDED
#define SVGFILE_H_INCLUDED

#include <string>
#include <fstream>
#include <set>

constexpr char defcol[] = "black";

class Svgfile
{
    public:
        Svgfile(bool showAxes, bool showNames, std::string _filename = "output.svg", int _width=1000, int _height=800);
        ~Svgfile();

        void addDisk(double x, double y, double r, std::string color=defcol);
        void addCircle(double x, double y, double r, double ep, double opacite, std::string color=defcol);
        void addRectangle(double x1, double y1, double x2, double y2, std::string colorFill, double opacite, std::string colorBordure);
        void addTriangle(double x1, double y1, double x2, double y2,
                         double x3, double y3, std::string colorFill,
                          double thickness, std::string colorStroke);
        void addTriangle(double x1, double y1, double x2, double y2,
                         double x3, double y3, std::string colorFill=defcol);
        void addLine(double x1, double y1, double x2, double y2, std::string color=defcol);
        void addLineGrid(double x1, double y1, double x2, double y2, std::string color);
        void addCross(double x, double y, double span, std::string color=defcol);

        void addText(double x, double y, std::string text, std::string color=defcol);
        void addText(double x, double y, double val, std::string color=defcol);
        void addTextGrid(double x, double y, std::string text, std::string color);

        void addGrid(double span=100.0, bool numbering=true, std::string color="lightgrey");

        static std::string makeRGB(int r, int g, int b);

        /// Type non copiable
        Svgfile(const Svgfile&) = delete;
        Svgfile& operator=(const Svgfile&) = delete;

        static bool s_verbose;

        bool getAffichageAxes();
        bool getAffichageNoms();

    private:
        std::string m_filename;
        std::ofstream m_ostrm;
        int m_width;
        int m_height;
        bool m_afficherAxes;
        bool m_afficherNoms;

        // Pour éviter les ouverture multiples
        static std::set<std::string> s_openfiles;
};

#endif // SVGFILE_H_INCLUDED



