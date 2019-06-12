// Copyright (c) 2018  Robert J. Hijmans
//
// This file is part of the "spat" library.
//
// spat is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// spat is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with spat. If not, see <http://www.gnu.org/licenses/>.

#include <vector>
#include <algorithm>
#include <string>
#include <cmath>

// comment out if GDAL not available
//#define useGDAL
// comment out if this is not for R (no Rcpp)
//#define useRcpp



#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

class SpatMessages {
	public:
		bool success = true;
		bool has_error = false;
		bool has_warning = false;
		std::string error;
		std::vector<std::string> warnings;

		void setError(std::string s) {
			has_error = true;
			error = s;
			success = false;
		}
		void addWarning(std::string s) {
			has_warning = true;
			warnings.push_back(s);
		}
};



class SpatOptions {
	private:
		std::string tempdir = "";
		bool todisk = false;
		double memfrac = 0.6;

	public:
		std::string def_datatype = "FLT4S";
		std::string def_filetype = "GTiff";
		bool overwrite = false;
		unsigned progress = 4;
		unsigned blocksizemp = 4;

		std::string datatype = "";
		std::string filetype = "";
		std::string filename = "";
		std::vector<std::string> gdal_options;

		SpatOptions();
		SpatOptions(const SpatOptions &opt);
		SpatOptions deepcopy(const SpatOptions &opt);

		// permanent
		bool get_todisk();
		void set_todisk(bool b);
		double get_memfrac();
		void set_memfrac(double d);
		std::string get_tempdir();
		void set_tempdir(std::string d);

		std::string get_def_datatype();
		std::string get_def_filetype();
		void set_def_datatype(std::string d);
		void set_def_filetype(std::string d);

		// single use
		void set_filename(std::string d);
		void set_filetype(std::string d);
		void set_datatype(std::string d);
		void set_overwrite(bool b);
		void set_progress(unsigned p);
		void set_blocksizemp(unsigned x);
		std::string get_filename();
		std::string get_filetype();
		std::string get_datatype();
		bool get_overwrite();
		unsigned get_progress();
		bool do_progress(unsigned n);
		unsigned get_blocksizemp();

		SpatMessages msg;
};



class SpatExtent {
	public:
		double xmin, xmax, ymin, ymax;
		double inf = std::numeric_limits<double>::infinity();
		double neginf = -std::numeric_limits<double>::infinity();
//		SpatExtent() {xmin = inf; xmax = neginf; ymin = inf; ymax = neginf;}
		SpatExtent() {xmin = -180; xmax = 180; ymin = -90; ymax = 90;}
		SpatExtent(double _xmin, double _xmax, double _ymin, double _ymax) {xmin = _xmin; xmax = _xmax; ymin = _ymin; ymax = _ymax;}

		void intersect(SpatExtent e) { // check first if intersects
			xmin = std::max(xmin, e.xmin);
			xmax = std::min(xmax, e.xmax);
			ymin = std::max(ymin, e.ymin);
			ymax = std::min(ymax, e.ymax);
		}

		void unite(SpatExtent e) {
			xmin = std::min(xmin, e.xmin);
			xmax = std::max(xmax, e.xmax);
			ymin = std::min(ymin, e.ymin);
			ymax = std::max(ymax, e.ymax);
		}

		std::vector<double> asVector() {
			std::vector<double> e = { xmin, xmax, ymin, ymax };
			return(e);
		}


		bool is_lonlat(std::string crs) {
			bool b1 = crs.find("longlat") != std::string::npos;
			bool b2 = crs.find("epsg:4326") != std::string::npos;
			return (b1 | b2);
		}

		bool could_be_lonlat(std::string crs) {
			bool b = is_lonlat(crs);
			if ((!b) & (crs=="")) {
				if ((xmin >=-180.1) & (xmax <= 180.1) & (ymin >= -90.1) & (ymax <= 90.1)) {
					b = true;
				}
			}
			return b;
		}

		bool is_global_lonlat(std::string crs) {
			if (could_be_lonlat(crs)) {
                // could be refined
                if (std::abs(xmax - xmin - 360) < 0.001) {
                    return true;
                }
            }
			return false;
		}

		bool valid() {
			return ((xmax > xmin) && (ymax > ymin));
		}
};





