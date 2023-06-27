#include <vector>
#include <iostream>
#include <unordered_set>

#include <pcl/io/pcd_io.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/common/common.h>

#include "../headers/BaseStruct.h"
#include "../headers/helper.h"
#include "../headers/validation.h"

Validation::Validation()
{
    // empty constructor
}

Validation::~Validation()
{
    // empty destructor
}


void Validation::raySampledCloud(double step, 
                                 double searchRadius, // search radius
                                 double sphereRadius, // radius of sphere
                                 size_t numSamples, // number of samples
                                 pcl::PointXYZ& minPt, 
                                 pcl::PointXYZ& maxPt,
                                 pcl::PointCloud<pcl::PointXYZ>::Ptr cloud,
                                 pcl::PointCloud<pcl::PointXYZRGB>::Ptr coloredCloud) 
{
    std::cout << "total rays: " << numSamples << std::endl;
    Helper helper;
    pcl::KdTreeFLANN<pcl::PointXYZ> kdtree;
    kdtree.setInputCloud(cloud);
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr sampledCloud(new pcl::PointCloud<pcl::PointXYZRGB>);

    pcl::PointXYZ center;
    center.x = (minPt.x + maxPt.x) / 2;
    center.y = (minPt.y + maxPt.y) / 2;
    center.z = (minPt.z + maxPt.z) / 2;
    std::unordered_set<int> addedPoints;
    std::vector<pcl::PointXYZ> sampledPoints = helper.UniformSamplingSphere(center, sphereRadius, numSamples);
    // store all index of points that should be added to the sampled cloud
    for (int i = 0; i < sampledPoints.size(); i++) {
        pcl::PointXYZ sampledPoint = sampledPoints[i];
        Ray3D ray = helper.generateRay(center, sampledPoint);
        while( sampledPoint.x < maxPt.x && sampledPoint.y < maxPt.y && sampledPoint.z < maxPt.z && sampledPoint.x > minPt.x && sampledPoint.y > minPt.y && sampledPoint.z > minPt.z) {
            std::vector<int> pointIdxRadiusSearch;
            std::vector<float> pointRadiusSquaredDistance;
            if ( kdtree.radiusSearch(sampledPoint, searchRadius, pointIdxRadiusSearch, pointRadiusSquaredDistance) > 0 ) {
                for (size_t i = 0; i < pointIdxRadiusSearch.size(); ++i) {
                    addedPoints.insert(pointIdxRadiusSearch[i]);
                }
            }
            sampledPoint.x += step * ray.direction.x;
            sampledPoint.y += step * ray.direction.y;
            sampledPoint.z += step * ray.direction.z;
        }
    }
    for (const auto& ptIdx : addedPoints) {
        pcl::PointXYZRGB point;
        point.x = cloud->points[ptIdx].x;
        point.y = cloud->points[ptIdx].y;
        point.z = cloud->points[ptIdx].z;
        point.r = coloredCloud->points[ptIdx].r;
        point.g = coloredCloud->points[ptIdx].g;
        point.b = coloredCloud->points[ptIdx].b;
        sampledCloud->push_back(point);
    }
    sampledCloud->width = sampledCloud->size();
    sampledCloud->height = 1;
    sampledCloud->is_dense = true;
    std::string outputPath = "../files/output/raySampledCloud";
    outputPath = outputPath + "_" + std::to_string(numSamples) + ".pcd";
    pcl::io::savePCDFileASCII (outputPath, *sampledCloud);

}


