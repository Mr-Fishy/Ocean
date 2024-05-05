#pragma once

namespace Ocean {

	class Log {
	public:
		enum class Level {
			INFO = 0,
			WARN = 1,
			ERROR = 2,
			ASSERT = 3
		};

		Log();
		~Log();

		bool Init();
		bool Shutdown();

		void Out(Log::Level errLevel);

	private:
		

		/* --- */

		#ifdef DEBUG
			friend class Validator;
		#endif // DEBUG
	};

}	// Ocean
