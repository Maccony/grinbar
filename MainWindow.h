#pragma once

#include <QMainWindow>
#include "ProcessData.h"
class QGridLayout;

class MainWindow : public QMainWindow {
    Q_OBJECT
    Q_DISABLE_COPY( MainWindow )
public:
    explicit MainWindow( QWidget*const parent = nullptr );

private:
    Q_SLOT void onAddProcess();
    Q_SLOT void onEditProcess( const ProcessData& );
    Q_SLOT void onRemoveProcess( const ProcessData& );

    virtual void moveEvent( QMoveEvent* ) override final;
    virtual void resizeEvent( QResizeEvent* ) override final;
    void writeGeometry();

    void addProcess( const ProcessData& );
    void removeProcess( const ProcessData& );

    void saveProcesses();
    void restoreProcesses();

private:
    QGridLayout*const m_grid = nullptr;
};
