#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/Log.h"
#include "cinder/gl/gl.h"

#include "game_controller.h"

using namespace ci;
using namespace ci::app;

auto prepareSettings = [](App::Settings* settings)
{
	log::makeLogger<log::LoggerFile>("pacman_app.log");
	settings->setWindowSize(500, 350);

#if defined( CINDER_COCOA_TOUCH )
	settings->setStatusBarEnabled(false); // FIXME: status bar is always visible?
#endif

#if defined( CINDER_MSW )
	settings->setConsoleWindowEnabled();
#endif
};

class PacmanApp : public App
{
public:
	void setup() override;
	void update() override;
	void draw() override;
	void keyDown(KeyEvent event) override;

private:
	Config mConfig;
	std::unique_ptr<GameController> mController;
	bool mGameActive{ false };
};

void PacmanApp::setup()
{
	mController = std::make_unique<GameController>(mConfig);
	mController->Setup();
}

void PacmanApp::keyDown(KeyEvent event)
{
	Direction direction{ Direction::NONE };
	switch (event.getCode())
	{
	case KeyEvent::KEY_DOWN:
		direction = Direction::DOWN; break;
	case KeyEvent::KEY_UP:
		direction = Direction::UP; break;
	case KeyEvent::KEY_LEFT:
		direction = Direction::LEFT; break;
	case KeyEvent::KEY_RIGHT:
		direction = Direction::RIGHT; break;
	case KeyEvent::KEY_SPACE:
		mController->SetGameActive(!mGameActive);
		mGameActive = !mGameActive;
		break;
	default:
		break;
	}
	mController->KeyDown(direction);
}

void PacmanApp::update()
{
	mController->Update(getElapsedSeconds());
}

void PacmanApp::draw()
{
	gl::clear(mConfig.BLACK);
	mController->Draw();
}


CINDER_APP(PacmanApp, RendererGl, prepareSettings)
