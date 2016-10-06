#pragma once
class Counter
{
public:
	Counter();
	Counter(int time_interval);
	~Counter();
	void ChangeInterval(float interval);
	void Update(float elapsed);
	void Reset();
	bool Passed() const;
private:
	float					time_interval;
	float					time_elapsed;
};

