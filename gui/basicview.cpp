#include "basicview.h"

#include <QVBoxLayout>

BasicView::BasicView()
{
    basicView = new QWidget();

    startButton = new QPushButton(tr("Start"));
    stopButton = new QPushButton(tr("Stop"));
    pauseButton = new QPushButton(tr("Pause"));
    deleteButton = new QPushButton(tr("Delete"));
    refreshButton = new QPushButton(tr("Refresh"));
    dockerEnvButton = new QPushButton(tr("docker-env"));
    dockerEnvButton->setToolTip(
            "Opens a terminal where the docker-cli points to docker engine inside "
            "minikube\n(Useful for building docker images directly inside minikube)");
    sshButton = new QPushButton(tr("SSH"));
    dashboardButton = new QPushButton(tr("Dashboard"));
    advancedButton = new QPushButton(tr("Advanced View"));

    disableButtons();

    QVBoxLayout *buttonLayout = new QVBoxLayout;
    basicView->setLayout(buttonLayout);
    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(stopButton);
    buttonLayout->addWidget(pauseButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(refreshButton);
    buttonLayout->addWidget(dockerEnvButton);
    buttonLayout->addWidget(sshButton);
    buttonLayout->addWidget(dashboardButton);
    buttonLayout->addWidget(advancedButton);
    basicView->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    connect(startButton, &QPushButton::clicked, this, &BasicView::start);
    connect(stopButton, &QAbstractButton::clicked, this, &BasicView::stop);
    connect(pauseButton, &QAbstractButton::clicked, this, &BasicView::pause);
    connect(deleteButton, &QAbstractButton::clicked, this, &BasicView::delete_);
    connect(refreshButton, &QAbstractButton::clicked, this, &BasicView::refresh);
    connect(dockerEnvButton, &QAbstractButton::clicked, this, &BasicView::dockerEnv);
    connect(sshButton, &QAbstractButton::clicked, this, &BasicView::ssh);
    connect(dashboardButton, &QAbstractButton::clicked, this, &BasicView::dashboard);
    connect(advancedButton, &QAbstractButton::clicked, this, &BasicView::advanced);
}

static QString getPauseLabel(bool isPaused)
{
    if (isPaused) {
        return "Unpause";
    }
    return "Pause";
}

static QString getStartLabel(bool isRunning)
{
    if (isRunning) {
        return "Restart";
    }
    return "Start";
}

void BasicView::update(Cluster cluster)
{
    startButton->setEnabled(true);
    advancedButton->setEnabled(true);
    refreshButton->setEnabled(true);
    bool exists = !cluster.isEmpty();
    bool isRunning = cluster.status() == "Running";
    bool isPaused = cluster.status() == "Paused";
    stopButton->setEnabled(isRunning || isPaused);
    pauseButton->setEnabled(isRunning || isPaused);
    deleteButton->setEnabled(exists);
    dashboardButton->setEnabled(isRunning);
#if __linux__ || __APPLE__
    dockerEnvButton->setEnabled(isRunning);
    sshButton->setEnabled(exists);
#else
    dockerEnvButton->setEnabled(false);
    sshButton->setEnabled(false);
#endif
    pauseButton->setText(getPauseLabel(isPaused));
    startButton->setText(getStartLabel(isRunning));
    QString startToolTip = "";
    if (isRunning) {
        startToolTip = "Restart an already running minikube instance to pickup config changes.";
    }
    startButton->setToolTip(startToolTip);
}

void BasicView::disableButtons()
{
    startButton->setEnabled(false);
    stopButton->setEnabled(false);
    deleteButton->setEnabled(false);
    pauseButton->setEnabled(false);
    dockerEnvButton->setEnabled(false);
    sshButton->setEnabled(false);
    dashboardButton->setEnabled(false);
    advancedButton->setEnabled(false);
    refreshButton->setEnabled(false);
}
