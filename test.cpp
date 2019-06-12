#include <stdio.h>
#include "src/SpatRaster.h"

#include <iostream>
#include <sstream>
#include <iterator>


std::string join(const std::vector<std::string>& vec, const char* delim) {
    std::stringstream s;
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<std::string>(s, delim));
    return s.str();
}


void show(SpatRaster &r) {
    std::cout << std::endl;
    if (r.msg.has_error) {
        std::cout << "Error: ";
        std::cout << r.msg.error << std::endl;
    } else {
        if (r.msg.has_warning) {
            std::cout << "Warning: ";
            std::cout << r.msg.warnings[0] << std::endl;
            r.msg.has_warning = false;
            r.msg.warnings.resize(0);
        }
      //  std::cout << r.source[0].filename << endl;
        std::cout << "dim     : " << r.nrow() << ", " << r.ncol() << ", " << r.nlyr() << std::endl;
        std::cout << "res     : " << r.xres() << ", " << r.yres() << std::endl;
        SpatExtent e = r.getExtent();
        std::cout << "extent  : " << e.xmin << ", " << e.xmax << ", " << e.ymin << ", " << e.ymax << std::endl;
        if (r.source[0].memory) {std::cout << "mem     : true" << std::endl;} else {
                std::cout << "filename: " << r.source[0].filename << std::endl;
        }
        std::string names = join(r.getNames(), ", ");
        std::cout << "names   : " << names << std::endl;
        std::vector<double> rmin = r.range_min();
        std::vector<double> rmax = r.range_max();
        std::cout << "min     : ";
        for (size_t i=0; i<rmin.size(); i++) { std::cout << rmin[i] << ", "; };
        std::cout << "\n";
        std::cout << "max     : ";
        for (size_t i=0; i<rmax.size(); i++) { std::cout << rmax[i] << ", "; };
        std::cout << "\n";
    }
}


void show(SpatVector &v) {
    std::cout << std::endl;
    if (v.msg.has_error) {
        std::cout << "Error: ";
        std::cout << v.msg.error << std::endl;
    } else {
        if (v.msg.has_warning) {
            std::cout << "Warning: ";
            std::cout << v.msg.warnings[0] << std::endl;
            v.msg.has_warning = false;
            v.msg.warnings.resize(0);
        }
      //  std::cout << r.source[0].filename << endl;
        std::cout << "dim     : " << v.nrow() << ", " << v.ncol() << std::endl;

        SpatExtent e = v.getExtent();
        std::cout << "extent  : " << e.xmin << ", " << e.xmax << ", " << e.ymin << ", " << e.ymax << std::endl;
        std::string names = join(v.names(), ", ");
        std::cout << "names   : " << names << std::endl;
        std::cout << "\n";
    }
}


void showValues(SpatRaster d) {
    std::vector<double> v = d.getValues();
    for (size_t i=0; i<d.nrow(); i++) {
        for (size_t j=0; j<d.ncol(); j++) {
            size_t cell = i*d.ncol()+j;
            if (std::isnan(v[cell])) {
                std::cout << "NA" << " ";
            } else {
                std::cout << v[cell] << " ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}


void showValues(std::vector<double> v, unsigned nr, unsigned nc) {
    for (size_t i=0; i<nr; i++) {
        for (size_t j=0; j<nc; j++) {
            size_t cell = i*nc+j;
            if (std::isnan(v[cell])) {
                std::cout << "NA" << " ";
            } else {
                std::cout << v[cell] << " ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void showValues(std::vector<std::vector<double>> v) {
    for (size_t i=0; i<v.size(); i++) {
        for (size_t j=0; j<v[i].size(); j++) {
            if (std::isnan(v[i][j])) {
                std::cout << "NA" << " ";
            } else {
                std::cout << v[i][j] << " ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void showValues(std::vector<std::vector<std::vector<double>>> v) {
    for (size_t i=0; i<v.size(); i++) {
        for (size_t j=0; j<v[i].size(); j++) {
            for (size_t k=0; k<v[i][j].size(); k++) {
                if (std::isnan(v[i][j][k])) {
                    std::cout << "NA" << " ";
                } else {
                    std::cout << v[i][j][k] << " ";
                }
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}



int main( int argc, const char* argv[] ) {
	printf( "\nHello World\n\n" );

	if (argc > 1) {
		SpatRaster ff(argv[1]);
		show(ff);
	} else {


		SpatOptions opts;
		SpatExtent e(0,10,0,5);
		SpatRaster r(2, 5, 3, e, "");
		std::vector<double> v;
		for (size_t j = 0; j < 6; j++) {
			for (size_t i=0; i<5; i++) {
				v.push_back(std::round(i/2));
			}
		}
		r.setValues(v);
		std::cout << "\nshow(r)\n";
		show(r);

		std::cout << "\nshowValues(r)\n";
		showValues(r);

		SpatRaster out = r.arith(1, ">", opts);
		std::cout << "\nSpatRaster out = r.arith(1, \">\", opts);\n";
		showValues(out);

		std::cout << "\nout = r.unique(0);\n";

		std::vector<std::vector<double>> u = r.unique(0);
		for (size_t i=0; i<u.size(); i++) {
		   for (size_t j=0; j<u[i].size(); j++) {
			   std::cout << u[i][j] << ", ";
			}
			std::cout << "\n";
		}
	 
		std::cout << "\nSpatRaster f(\"r.grd\");\n";
		SpatRaster f("r.grd");
		show(f);
	}
}

