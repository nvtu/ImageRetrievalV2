//
//  ir_instance.h
//  server
//
//  Created by Bao Truong on 7/3/17.
//
//

#ifndef ir_instance_h
#define ir_instance_h

#include <arrayfire.h>
#include <boost/thread.hpp>
#include <flann/flann.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

#include "database_params.h"
#include "../env_params.h"
#include "ir_result.h"
#include "quantization_params.h"

/**
 * A thread safe image retrieval system.
 */
class IrInstance {
 private:

  // Static variables
  static IrInstance* instance_;
  static boost::mutex initMutex_;

  // Documents
  std::vector<std::string> docNames_;

  // Quantization variables
  flann::Index< flann::L2<float> >* quantIndex_;
  QuantizationParams quantParams_;

  // Bag-of-word variables
  af::array database_;
  af::array invIndex_;
  af::array invDocFreq_;
  DatabaseParams dbParams_;

  IrInstance();

  // Methods

  void createInstanceIfNecessary();

  void buildIndexIfNecessary(bool overwrite = env_params::overwrite());

  void buildDatabase(
    bool overwrite = env_params::overwrite(),
    size_t nThreads = env_params::no_threads());

  void quantize(
    const af::array &descriptors,
    std::vector<size_t> &termIndices,
    std::vector<float> &termWeights);

  void computeTF(
    const std::vector<size_t> &indices,
    const std::vector<float> &weights,
    af::array &termFreq);

  void computeScore(
    const af::array& bow,
    std::vector<float> &scores);

 public:

  /**
   * Retrieve a list of simiar image in the database along with their score.
   */
  std::vector<IrResult> retrieve(const cv::Mat& image, int topK = -1);
};

IrInstance* IrInstance::instance_ = NULL;
boost::mutex IrInstance::initMutex_;

#endif /* ir_instance_h */