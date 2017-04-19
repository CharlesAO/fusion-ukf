#include "tests.h"


/*******************************************
 * STATE PREDICTOR TESTS
 *******************************************/

bool test_compute_augmented_sigma(){

  double SMALL_POSITIVE_VALUE = 1.e-05;
  VectorXd WEIGHTS = VectorXd(NSIGMA);
  WEIGHTS << w0, w, w, w, w, w, w, w, w, w, w, w, w, w, w;
  StatePredictor statePredictor(WEIGHTS);

  VectorXd x = VectorXd(NX);
  x << 5.7441,
       1.3800,
       2.2049,
       0.5015,
      0.3528;

  MatrixXd P = MatrixXd(NX, NX);
  P <<  0.0043,   -0.0013,    0.0030,   -0.0022,   -0.0020,
       -0.0013,    0.0077,    0.0011,    0.0071,    0.0060,
        0.0030,    0.0011,    0.0054,    0.0007,    0.0008,
       -0.0022,    0.0071,    0.0007,    0.0098,    0.0100,
       -0.0020,    0.0060,    0.0008,    0.0100,    0.0123;

  MatrixXd sigma_x = statePredictor.compute_augmented_sigma(x, P);

  MatrixXd expected_sigma_x = MatrixXd(NAUGMENTED, NSIGMA);

  expected_sigma_x <<
     5.7441,  5.85768,  5.7441,   5.7441,   5.7441,   5.7441,   5.7441,   5.7441,  5.63052,  5.7441,   5.7441,   5.7441,   5.7441,   5.7441,   5.7441,
     1.38,    1.34566,  1.52806,  1.38,     1.38,     1.38,     1.38,     1.38,    1.41434,  1.23194,  1.38,     1.38,     1.38,     1.38,     1.38,
     2.2049,  2.28414,  2.24557,  2.29582,  2.2049,   2.2049,   2.2049,   2.2049,  2.12566,  2.16423,  2.11398,  2.2049,   2.2049,   2.2049,   2.2049,
     0.5015,  0.44339,  0.631886, 0.516923, 0.595227, 0.5015,   0.5015,   0.5015,  0.55961,  0.371114, 0.486077, 0.407773, 0.5015,   0.5015,   0.5015,
     0.3528,  0.299973, 0.462123, 0.376339, 0.48417,  0.418721, 0.3528,   0.3528,  0.405627, 0.243477, 0.329261, 0.22143,  0.286879, 0.3528,   0.3528,
     0,       0,        0,        0,        0,        0,        0.34641,  0,       0,        0,        0,        0,        0,       -0.34641,  0,
     0,       0,        0,        0,        0,        0,        0,        0.34641, 0,        0,        0,        0,        0,        0,       -0.34641;

  //cout << "sigma_x" << endl;
  //cout << sigma_x << endl;
  //cout << "expected_sigma_x" << endl;
  //cout << expected_sigma_x << endl;

  bool r =  (expected_sigma_x - sigma_x).norm() < SMALL_POSITIVE_VALUE;
  return r;
}

bool test_predict_sigma_x(){

  double SMALL_POSITIVE_VALUE = 1.e-04;
  VectorXd WEIGHTS = VectorXd(NSIGMA);
  WEIGHTS << w0, w, w, w, w, w, w, w, w, w, w, w, w, w, w;
  StatePredictor statePredictor(WEIGHTS);

  MatrixXd augmented_sigma_x = MatrixXd(NAUGMENTED, NSIGMA);
  augmented_sigma_x <<
    5.7441,  5.85768,   5.7441,   5.7441,   5.7441,   5.7441,   5.7441,   5.7441,   5.63052,   5.7441,   5.7441,   5.7441,   5.7441,   5.7441,   5.7441,
      1.38,  1.34566,  1.52806,     1.38,     1.38,     1.38,     1.38,     1.38,   1.41434,  1.23194,     1.38,     1.38,     1.38,     1.38,     1.38,
    2.2049,  2.28414,  2.24557,  2.29582,   2.2049,   2.2049,   2.2049,   2.2049,   2.12566,  2.16423,  2.11398,   2.2049,   2.2049,   2.2049,   2.2049,
    0.5015,  0.44339, 0.631886, 0.516923, 0.595227,   0.5015,   0.5015,   0.5015,   0.55961, 0.371114, 0.486077, 0.407773,   0.5015,   0.5015,   0.5015,
    0.3528, 0.299973, 0.462123, 0.376339,  0.48417, 0.418721,   0.3528,   0.3528,  0.405627, 0.243477, 0.329261,  0.22143, 0.286879,   0.3528,   0.3528,
         0,        0,        0,        0,        0,        0,  0.34641,        0,         0,        0,        0,        0,        0, -0.34641,        0,
         0,        0,        0,        0,        0,        0,        0,  0.34641,         0,        0,        0,        0,        0,        0, -0.34641;

  MatrixXd predicted_sigma_x = statePredictor.predict_sigma(augmented_sigma_x, 0.1);

  MatrixXd expected_predicted_sigma_x = MatrixXd(NX, NSIGMA);
  expected_predicted_sigma_x <<
  5.93553, 6.06251,  5.92217,  5.9415,   5.92361,  5.93516,  5.93705, 5.93553,  5.80832,  5.94481,  5.92935,  5.94553,  5.93589,  5.93401, 5.93553,
  1.48939, 1.44673,  1.66484,  1.49719,  1.508,    1.49001,  1.49022, 1.48939,  1.5308,   1.31287,  1.48182,  1.46967,  1.48876,  1.48855, 1.48939,
  2.2049,  2.28414,  2.24557,  2.29582,  2.2049,   2.2049,   2.23954, 2.2049,   2.12566,  2.16423,  2.11398,  2.2049,   2.2049,   2.17026, 2.2049,
  0.53678, 0.473387, 0.678098, 0.554557, 0.643644, 0.543372, 0.53678, 0.538512, 0.600173, 0.395462, 0.519003, 0.429916, 0.530188, 0.53678, 0.535048,
  0.3528,  0.299973, 0.462123, 0.376339, 0.48417,  0.418721, 0.3528,  0.387441, 0.405627, 0.243477, 0.329261, 0.22143,  0.286879, 0.3528,  0.318159;

  //cout << "predicted_sigma" << endl;
  //cout << predicted_sigma_x << endl;
  //cout << "expected_predicted_sigma" << endl;
  //cout << expected_predicted_sigma_x << endl;

  bool r =  (expected_predicted_sigma_x - predicted_sigma_x).norm() < SMALL_POSITIVE_VALUE;
  return r;
}
