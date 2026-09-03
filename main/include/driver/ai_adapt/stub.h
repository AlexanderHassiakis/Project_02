/**
 * @file Linear regression model interface.
 */
#pragma once
#include "driver/ai_adapt/interface.h"
namespace driver::ai_adapt
{
	/**
	 * @brief Linear regression model interface.
	 */
	class Interface
	{
	public:
		/**
		 * @brief Destructor.
		 */
		~Interface() noexcept = default;

		/**
		 * @brief Predict based on the given input.
		 *
		 * @param[in] input Input to predict with.
		 *
		 * @return Predicted output.
		 */
		[[nodiscard]] virtual double predict(double input) const noexcept = 0;
	};
} // namespace driver::ai_adapt