#ifndef BKEMULVIDEO_H
#define BKEMULVIDEO_H

#if _MSC_VER > 1000
#pragma once
#endif

namespace bkemul {

// --------------------------------------------------------------
// ---------- BKEmulVideo
// --------------------------------------------------------------
class BKEmulVideo
{
private:
	bool colorMonitor;
	bool smallScreen;

public:
	BKEmulVideo();
	virtual ~BKEmulVideo();

	const BYTE* getMemory( WORD address = 040000 ) const;
	int getMemorySize() const           { return 040000;       }

	BYTE getMemoryByte( WORD address );
	WORD getMemoryWord( WORD address );
	void setMemoryByte( WORD address, BYTE data );
	void setMemoryWord( WORD address, WORD data );

	bool isColorMonitor() const                { return colorMonitor; }
	void setColorMonitor( const bool value );

	void updateMonitor() const;
	void updateScrolling( BYTE delta ) const;
	void updateVideoMemoryByte( WORD address ) const;
	void updateVideoMemoryWord( WORD address ) const;

	bool isSmallScreen() const                 { return smallScreen; }
	void setSmallScreen( const bool value );
};

} // namespace bkemul

#endif // BKEMULVIDEO_H
