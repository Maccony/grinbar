#include "ProcessForm.h"
#include <QLabel>
#include <QToolButton>
#include <QPushButton>
#include <QVBoxLayout>
#include <QProcess>

ProcessForm::ProcessForm( QWidget*const parent )
    : QFrame( parent )
    , m_title( new QLabel( this ) )
    , m_description( new QLabel( this ) )
{
    setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );

    auto layout = new QVBoxLayout( this );

    auto h_layout = new QHBoxLayout;
    layout->addLayout( h_layout );

    auto font = m_title->font();
    font.setBold( true );
    font.setPointSizeF( font.pointSizeF() * 1.5 );
    m_title->setFont( font );
    h_layout->addWidget( m_title );

    auto edit_button = new QToolButton();
    edit_button->setText( tr( "Edit" ) );
    edit_button->setIcon( QIcon( ":/pencil.png" ) );
    edit_button->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
    connect( edit_button, &QPushButton::clicked, this, &ProcessForm::onEdit );
    h_layout->addWidget( edit_button);

    auto remove_button= new QToolButton();
    remove_button->setText( tr( "Remove" ) );
    remove_button->setIcon( QIcon( ":/trash.png" ) );
    remove_button->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
    connect( remove_button, &QPushButton::clicked, this, &ProcessForm::onRemove );
    h_layout->addWidget( remove_button);

    m_description->setWordWrap( true );
    m_description->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Minimum );
    layout->addWidget( m_description );

    auto run_button = new QPushButton( tr( "Run" ) );
    connect( run_button, &QPushButton::clicked, this, &ProcessForm::onRun );
    layout->addWidget( run_button, Qt::AlignCenter );
}

void ProcessForm::setProcessData( const ProcessData& data ) {
    m_data = data;
    m_title->setText( data.title );
    m_description->setText( data.description );
}

ProcessData ProcessForm::processData() const {
    return m_data;
}

void ProcessForm::onRun() {
    system( m_data.command.toLocal8Bit().constData() );
}

void ProcessForm::onEdit() {
    emit edit( m_data );
}

void ProcessForm::onRemove() {
    emit remove( m_data );
}

