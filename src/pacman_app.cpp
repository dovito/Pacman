#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/Log.h"
#include "cinder/gl/gl.h"

#include "game_controller.h"

using namespace ci;
using namespace ci::app;

using namespace std;

void prepareSettings(App::Settings* settings)
{
	log::makeLogger<log::LoggerFile>("pacmanApp.log");
	settings->setWindowSize(600, 600);

#if defined( CINDER_COCOA_TOUCH )
	settings->setStatusBarEnabled(false); // FIXME: status bar is always visible?
#endif

#if defined( CINDER_MSW )
	settings->setConsoleWindowEnabled();
#endif

}

class PacmanApp : public App
{
public:
	void setup() override;
	//void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
	void keyDown(KeyEvent event) override;
	void keyUp(KeyEvent event) override {}

private:
	Config mConfig;
	std::unique_ptr<GameController> mController;
	double mLastTime = 0;
	int mUpdateCount = 0;
};

void PacmanApp::setup()
{
	CI_LOG_I("height" << getWindowHeight() << " width=" << getWindowWidth());
	mController = std::make_unique<GameController>(mConfig);
	mController->setup();
}

void PacmanApp::keyDown(KeyEvent event)
{
	switch (event.getCode())
	{
	case KeyEvent::KEY_DOWN:
		CI_LOG_I("keyDown"); break;
	case KeyEvent::KEY_UP:
		CI_LOG_I("keyUp"); break;
	case KeyEvent::KEY_LEFT:
		CI_LOG_I("keyLeft"); break;
	case KeyEvent::KEY_RIGHT:
		CI_LOG_I("keyRight"); break;
	default:
		break;
	}
}

void PacmanApp::update()
{
	double now = getElapsedSeconds();
	if (mLastTime > 0)
	{
		double delta = now - mLastTime;
		mController->update(delta);
	}
	mLastTime = now;
	
}

void PacmanApp::draw()
{
	gl::clear(mConfig.BLACK);
	mController->draw();
}


CINDER_APP(PacmanApp, RendererGl, prepareSettings)
