#pragma once
#include "common.h"

class Button
{
public:
	Button(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed);
	~Button() = default;

	void ProcessEvent(const ExMessage& msg);
	void Draw();
	void ResetStatus();

protected:
	virtual void OnClick() = 0;

private:
	enum class Status
	{
		Idle = 0,
		Hovered,
		Pushed,
	};

	RECT region;
	IMAGE img_idle;
	IMAGE img_hovered;
	IMAGE img_pushed;
	Status status = Status::Idle;

	bool CheckCursorHit(int x, int y);
};

class StartGameButton : public Button
{
public:
	StartGameButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed);
	~StartGameButton() = default;

protected:
	void OnClick() override;
};

class QuitGameButton : public Button
{
public:
	QuitGameButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed);
	~QuitGameButton() = default;

protected:
	void OnClick() override;
};

class SettingsButton : public Button
{
public:
	SettingsButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed);
	~SettingsButton() = default;

protected:
	void OnClick() override;
};

class BackButton : public Button
{
public:
	BackButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed);
	~BackButton() = default;

protected:
	void OnClick() override;
};
