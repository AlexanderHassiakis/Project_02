/**
 * @file Linear regression model with fixed learning rate.
 */
#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <exception>
#include <cmath>

#include "driver/ai_adapt/esp32s3.h"


namespace driver::ai_adapt
{
	namespace
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
	} // namespace

	// -----------------------------------------------------------------------------
	Fixed::Fixed(const Matrix1d &trainIn, const Matrix1d &trainOut) noexcept
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
	double Fixed::predict(const double input) const noexcept { return myWeight * input + myBias; }

	// -----------------------------------------------------------------------------
	bool Fixed::train(std::size_t epochCount, double learningRate,
					  double precisionThreshold) noexcept
	{
		constexpr std::size_t evaluationInterval{10};
		// Check epoch count, return false if 0.
		if (0U == epochCount)
		{
			return false;
		}

		// Check learning rate, return false if outside range (0.0, 1.0).
		if ((0.0 >= learningRate) || (1.0 <= learningRate))
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
			}
		}
		return true;
	}

	// -----------------------------------------------------------------------------
	void Fixed::optimize(const double input, const double output, const double learningRate) noexcept
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
	void Fixed::shuffle() noexcept
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
	double Fixed::computePrecision() const noexcept
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
