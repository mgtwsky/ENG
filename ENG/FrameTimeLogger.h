#pragma once

#include <array>
#include <fstream>

class FrameTimeLogger {
public:

	FrameTimeLogger() {}

	~FrameTimeLogger() {}

	void Start() {
		if (started) return;

		reset();
		started = true;
	}

	void Stop() {
		if (!started) return;

		started = false;
		SaveToFile();
	}

	void Update(float const & elapsed) {
		if (!started) return;

		if (index == 1000) {
			Stop();
			return;
		}

		frame_times[index++] = elapsed;
	}

	bool IsStarted() const { return started; }

private:
	void reset() {
		frame_times.fill(0.f);
		index = 0;
	}

	void SaveToFile() {
		Platform::String^ localfolder = Windows::Storage::ApplicationData::Current->LocalFolder->Path;

		std::wstring folderNameW(localfolder->Begin());
		std::string folderNameA(folderNameW.begin(), folderNameW.end());
		std::string fileName = folderNameA + "\\DataLog.txt";

		std::ofstream out(fileName, std::ios::out);
		if (out) {
			for (const auto & frame_time : frame_times) {
				if (frame_time > 0)
					out << std::to_string(frame_time) + "\n";
			}

			out.close();
		}
	}

	bool started = false;
	int index;
	std::array<float, 1000> frame_times;
};

