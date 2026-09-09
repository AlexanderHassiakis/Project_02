/**
 * @file Linear regression model interface.
 */
#pragma once
#include "driver/ai_adapt/interface.h"
#include "driver/ai_adapt/matrix.h"

namespace driver::ai_adapt
{
/**
 * @brief Linear regression model interface.
 */
class Esp32s3 final : public Interface
{
public:
    /**
     * @brief Constructor.
     *
     * @param[in] trainIn Training inputs. Size must be greater than 0.
     * @param[in] trainOut Training outputs. Size must be greater than 0.
     *
     * @note The user is responsible for the lifetime of the training data; the training data
     *       must be accessible during the training cycle. The training data must not change
     *       during training, else the performance might suffer.
     */
    explicit Esp32s3(const Matrix1d& trainIn, const Matrix1d& trainOut) noexcept;

    /**
     * @brief Destructor.
     */
    ~Esp32s3() noexcept override = default;

    /**
     * @brief Predict based on the given input.
     *
     * @param[in] input Input to predict with.
     *
     * @return Predicted output.
     */
    [[nodiscard]] double predict(double input) const noexcept override;

    /**
     * @brief Train the model for the given number of epochs.
     *
     * @param[in] epochCount Number of epochs to train the model. Must be greater than 0.
     * @param[in] learningRate Learning rate to use. Must be in range (0.0, 1.0), non-inclusive.
     *
     * @return True if training was performed, false if input argument is invalid.
     */
    bool train(std::size_t epochCount, double precisionThreshold = 0.99999) noexcept;

    Esp32s3()                          = delete; // No default constructor.
    Esp32s3(const Esp32s3&)            = delete; // No copy constructor.
    Esp32s3(Esp32s3&&)                 = delete; // No move constructor.
    Esp32s3& operator=(const Esp32s3&) = delete; // No copy assignment.
    Esp32s3& operator=(Esp32s3&&)      = delete; // No move assignment.

private:
    /**
     * @brief Adjust the trainable parameters.
     *
     * @param[in] input Input value (x).
     * @param[in] output Output value (yref).
     * @param[in] learningRate Learning rate.
     */
    void optimize(double input, double output, double learningRate) noexcept;

    /**
     * @brief Shuffle training order index list.
     */
    void shuffle() noexcept;

    /** Matrix holding the training order via index. */
    MatrixU32 myTrainOrder;

    /** Matrix holding training inputs. */
    const Matrix1d& myTrainIn;

    /** Matrix holding training outputs. */
    const Matrix1d& myTrainOut;

    /** Bias value (m). */
    double myBias;

    /** Weight value (k). */
    double myWeight;
};
} // namespace driver::ai_adapt