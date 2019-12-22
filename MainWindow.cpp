#include "MainWindow.h"
#include "ProcessDialog.h"
#include "ProcessForm.h"
#include <QToolBar>
#include <QAction>
#include <QMenu>
#include <QGridLayout>
#include <QSettings>
#include <QMessageBox>
#include <QPushButton>

namespace {
    const QString SETTINGS_KEY_GEOMETRY( "geometry" );
}

MainWindow::MainWindow( QWidget* parent )
    : QMainWindow( parent )
    , m_grid( new QGridLayout )
{
    setCentralWidget( new QWidget( this ) );
    centralWidget()->setLayout( m_grid );

    auto toolbar = addToolBar( tr( "File" ) );

    auto add_action = toolbar->addAction( "&Add" );
    connect( add_action, &QAction::triggered, this, &MainWindow::onAddProcess );

    QSettings settings;
    if ( settings.contains( SETTINGS_KEY_GEOMETRY ) )  {
        auto prev_geometry = settings.value( SETTINGS_KEY_GEOMETRY ).toByteArray();
        restoreGeometry( prev_geometry );
    }

    restoreProcesses();
}

void MainWindow::onAddProcess() {
    ProcessDialog dialog;
    if ( QDialog::Accepted == dialog.exec() ) {
        addProcess( dialog.data() );
        saveProcesses();
    }
}

void MainWindow::onEditProcess( const ProcessData& data ) {
    ProcessDialog dialog;
    dialog.setData( data );
    if ( QDialog::Accepted != dialog.exec() ) {
        return;
    }

    const auto new_data = dialog.data();
    bool moved = ( data.row != new_data.row );
    moved = moved || ( data.column != new_data.column );
    if ( moved ) {
        removeProcess( data );
        addProcess( new_data );
    } else {
        auto form = qobject_cast< ProcessForm* >( sender() );
        Q_ASSERT( form );
        form->setProcessData( new_data );
    }

    saveProcesses();
}

void MainWindow::onRemoveProcess( const ProcessData& data ) {
    QMessageBox dialog;
    dialog.setWindowTitle( tr( "Remove Command" ) );
    dialog.setText( tr( "Please, confirm removing process:"
                        "\n\t Title: %1"
                        "\n\t Command: %2" )
                        .arg( data.title, data.command ) );
    dialog.setIcon( QMessageBox::Question );
    dialog.addButton( QMessageBox::Yes );
    dialog.addButton( QMessageBox::No );
    dialog.setDefaultButton( QMessageBox::No );
    if ( QMessageBox::Yes == dialog.exec() ) {
        removeProcess( data );
        saveProcesses();
    }
}

void MainWindow::moveEvent( QMoveEvent* ) {
    writeGeometry();
}

void MainWindow::resizeEvent( QResizeEvent* ) {
    writeGeometry();
}

void MainWindow::writeGeometry() {
    QSettings settings;
    settings.setValue( SETTINGS_KEY_GEOMETRY, saveGeometry() );
}

void MainWindow::addProcess( const ProcessData& data ) {
    auto item = m_grid->itemAtPosition( data.row, data.column );
    if ( !item || !item->widget() ) {
        auto widget = new ProcessForm( this );
        widget->setProcessData( data );
        connect( widget, &ProcessForm::edit, this, &MainWindow::onEditProcess );
        connect( widget, &ProcessForm::remove, this, &MainWindow::onRemoveProcess );
        m_grid->addWidget( widget, data.row, data.column );
        return;
    }

    auto widget = item->widget();
    auto form = qobject_cast< ProcessForm* >( widget );
    Q_ASSERT( form );
    form->setProcessData( data );
}

void MainWindow::removeProcess( const ProcessData& data ) {
    auto item_by_position = m_grid->itemAtPosition( data.row, data.column );
    for ( int i = 0; i < m_grid->count(); ++i ) {
        auto item_by_index = m_grid->itemAt( i );
        if ( item_by_index == item_by_position ) {
            auto item = m_grid->takeAt( i );
            Q_ASSERT( item->widget() );
            item->widget()->deleteLater();
            return;
        }
    }
}

namespace {
    const QString SETTINGS_PROCESS_ARRAY( "Processes" );
    const QString SETTINGS_PROCESS_ROW_KEY( "row" );
    const QString SETTINGS_PROCESS_COLUMN_KEY( "column" );
    const QString SETTINGS_PROCESS_TITLE_KEY( "title" );
    const QString SETTINGS_PROCESS_DESCRIPTION_KEY( "description" );
    const QString SETTINGS_PROCESS_COMMAND_KEY( "command" );
}

void MainWindow::saveProcesses() {
    QSettings settings;
    settings.beginWriteArray( SETTINGS_PROCESS_ARRAY );
    int index = 0;
    for ( int row = 0; row < m_grid->rowCount(); ++row ) {
        for ( int column = 0; column < m_grid->columnCount(); ++column ) {

            auto item = m_grid->itemAtPosition( row, column );
            if ( not item ) {
                continue;
            }

            auto widget = item->widget();
            if ( not widget ) {
                continue;
            }

            auto form = qobject_cast< ProcessForm* >( widget );
            Q_ASSERT( form );

            auto data = form->processData();

            settings.setArrayIndex( index++ );
            settings.setValue( SETTINGS_PROCESS_ROW_KEY, row );
            settings.setValue( SETTINGS_PROCESS_COLUMN_KEY, column );
            settings.setValue( SETTINGS_PROCESS_TITLE_KEY, data.title );
            settings.setValue( SETTINGS_PROCESS_DESCRIPTION_KEY, data.description );
            settings.setValue( SETTINGS_PROCESS_COMMAND_KEY, data.command );

        }
    }
    settings.endArray();
}

void MainWindow::restoreProcesses() {
    QSettings settings;
    const int count = settings.beginReadArray( SETTINGS_PROCESS_ARRAY );
    for ( int index = 0; index < count; ++index ) {
        ProcessData data;
        settings.setArrayIndex( index );
        data.row = settings.value( SETTINGS_PROCESS_ROW_KEY ).toInt();
        data.column = settings.value( SETTINGS_PROCESS_COLUMN_KEY ).toInt();
        data.title = settings.value( SETTINGS_PROCESS_TITLE_KEY ).toString();
        data.description = settings.value( SETTINGS_PROCESS_DESCRIPTION_KEY ).toString();
        data.command = settings.value( SETTINGS_PROCESS_COMMAND_KEY ).toString();
        addProcess( data );
    }
    settings.endArray();
}
