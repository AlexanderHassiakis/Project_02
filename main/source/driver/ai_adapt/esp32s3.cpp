/**
 * @file Linear regression model with Esp32s3 learning rate.
 */
#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <exception>
#include <cmath>

#include "driver/ai_adapt/esp32s3.h"
#include "driver/ai_adapt/interface.h"
#include "driver/ai_adapt/matrix.h"


namespace driver::ai_adapt
{
	
	// -----------------------------------------------------------------------------
	void initRandom() noexcept
	{
		// Only initialize the random generator once.
		static bool initialized{false};
		if (initialized)
		{
			return;
		}

		// Initialize the random generator with the current timestamp as seed.
		std::srand(std::time(nullptr));
		initialized = true;
	}

	// -----------------------------------------------------------------------------
	Esp32s3::Esp32s3(const Matrix1d &trainIn, const Matrix1d &trainOut) noexcept
		: myTrainOrder{}, myTrainIn{trainIn}, myTrainOut{trainOut}, myBias{}, myWeight{}
	{
		const auto setCount = std::min(trainIn.size(), trainOut.size());

		if (0U == setCount)
		{
			std::fprintf(stderr, "Cannot create regression model without training data!\n");
			std::terminate();
		}

		// Initialize training order vector with indexes of the training sets.
		myTrainOrder.resize(setCount);

		for (std::uint32_t i{}; i < setCount; ++i)
		{
			myTrainOrder[i] = i;
		}

		// Initialize random generator (occurs only once).
		initRandom();
	}

	// -----------------------------------------------------------------------------
	double Esp32s3::predict(const double input) const noexcept { return myWeight * input + myBias; }

	// -----------------------------------------------------------------------------
	bool Esp32s3::train(std::size_t epochCount,double precisionThreshold) noexcept
	{
		double prevPrecision = 0.0;
		double learningRate = 0.1;
		constexpr std::size_t evaluationInterval{10};
		// Check epoch count, return false if 0.
	
		if (0U == epochCount)
		{
			return false;
		}

		// Check precision threshold, return false if outside range (0.0 1.0)
		if ((0.0 >= precisionThreshold) || (1.0 <= precisionThreshold))
		{
			return false;
		}

		for (std::size_t epoch{}; epoch < epochCount; ++epoch)
		{
			shuffle();

			for (const auto i : myTrainOrder)
			{
				const auto input = myTrainIn[i];
				const auto output = myTrainOut[i];
				optimize(input, output, learningRate);
			}
			// Evaluate the precision every 10th epoch, skip the first one.
			const auto evaluate = ((0U < epoch) && (0U == (epoch % evaluationInterval)));
			if (evaluate)
			{
				// Compute precision , stop training and print results .
				const auto precision = computePrecision();
				if (precision)
				{
					std::printf("Finished training with precision %g after %zu epochs!\n", precision, epoch);
					return true;
				}
				
				const auto diffPrecision = precision - prevPrecision;
				
				if (diffPrecision < 0)
				{
					learningRate = learningRate * 0.9;
				}

				else if (diffPrecision < 0.1)
				{
					learningRate = learningRate * 1.1;
				}
				learningRate = std::clamp(learningRate, 0.01, 0.25);
				prevPrecision = precision;
			}
		}
		return true;
	}

	// -----------------------------------------------------------------------------
	void Esp32s3::optimize(const double input, const double output, const double learningRate) noexcept
	{
		// m == yref if x == 0.
		if (0.0 == input)
		{
			myBias = output;
			return;
		}

		const auto prediction = predict(input);	  // yp = kx + m.
		const auto error = output - prediction;	  // e  = yref - yp.
		myBias += error * learningRate;			  // m  = m + e * LR
		myWeight += error * learningRate * input; // k  = k + e * LR * x
	}

	// -----------------------------------------------------------------------------
	void Esp32s3::shuffle() noexcept
	{
		// Iterate through all training sets, swap each index i with a random index r.
		for (std::size_t i{}; i < myTrainOrder.size(); ++i)
		{
			const auto r = std::rand() % myTrainOrder.size();
			const auto temp = myTrainOrder[i];
			myTrainOrder[i] = myTrainOrder[r];
			myTrainOrder[r] = temp;
		}
	}
	// -----------------------------------------------------------------------------
	double Esp32s3::computePrecision() const noexcept
	{
		double sum{0};
		// Iterate through all training sets.

		// compute the current error.
		for (std::size_t i{}; i < myTrainOrder.size(); i++)
		{
			const auto input = myTrainIn[i];
			const auto output = myTrainOut[i];
			const auto predicton = predict(input);

			const auto error = std::abs(output - predicton);
			sum += error;
		}

		const auto avgError = sum / myTrainOrder.size();
		return 1.0 - avgError;

		// Accumulate each error value => add it to the sum.
		// Return 1.0 -average error, where average error = sum/setCount.
	}
} // driver::ai_adapt
