#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include "BaseStruct.h"


class Scanner {

    private:
        /* data */
    public:
        Scanner();

        ~Scanner();

        pcl::PointCloud<pcl::PointXYZRGB>::Ptr multi_sphere_scanner(double step, 
                                                                    double searchRadius,
                                                                    double sphereRadius, 
                                                                    size_t num_samples,
                                                                    pcl::PointXYZ& minPt, 
                                                                    pcl::PointXYZ& maxPt,
                                                                    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud,
                                                                    pcl::PointCloud<pcl::PointXYZRGB>::Ptr coloredCloud,
                                                                    double density,
                                                                    std::string file_name);

        std::vector<pcl::PointXYZ> scanning_positions(size_t num_positions, 
                                                        pcl::PointXYZ& minPt, 
                                                        pcl::PointXYZ& maxPt,
                                                        int pattern);
        
        std::vector<pcl::PointXYZ> sample_square_points(const pcl::PointXYZ& scanner_position , double distance, double angle);


        pcl::PointCloud<pcl::PointXYZRGB>::Ptr scan_cloud(double step, 
                                                        double searchRadius, // search radius
                                                        pcl::PointXYZ& minPt, 
                                                        pcl::PointXYZ& maxPt,
                                                        pcl::PointCloud<pcl::PointXYZ>::Ptr cloud,
                                                        pcl::PointCloud<pcl::PointXYZRGB>::Ptr coloredCloud,
                                                        std::string file_name);
};

