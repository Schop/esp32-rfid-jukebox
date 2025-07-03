# Contributing to ESP32 RFID Jukebox

Thank you for your interest in contributing to the ESP32 RFID Jukebox project! This guide will help you get started.

## 🤝 How to Contribute

### Reporting Issues
1. **Search existing issues** first to avoid duplicates
2. **Use issue templates** when available
3. **Provide detailed information**:
   - Hardware configuration
   - Software version
   - Complete error messages
   - Steps to reproduce
   - Expected vs actual behavior

### Suggesting Features
1. **Check roadmap** for planned features
2. **Open feature request** with detailed description
3. **Explain use case** and benefits
4. **Consider implementation complexity**

### Contributing Code
1. **Fork the repository**
2. **Create feature branch** (`git checkout -b feature/amazing-feature`)
3. **Make your changes** following coding standards
4. **Test thoroughly** on actual hardware
5. **Update documentation** if needed
6. **Commit changes** (`git commit -m 'Add amazing feature'`)
7. **Push to branch** (`git push origin feature/amazing-feature`)
8. **Open Pull Request**

## 🛠️ Development Setup

### Prerequisites
- **VS Code** with PlatformIO extension
- **ESP32 development board**
- **Hardware components** (see README.md)
- **Git** for version control

### Local Development
```bash
# Clone your fork
git clone https://github.com/yourusername/esp32-rfid-jukebox.git
cd esp32-rfid-jukebox

# Create branch for your feature
git checkout -b feature/your-feature-name

# Install dependencies (automatic with PlatformIO)
# Make changes and test
# Commit and push changes
```

### Testing Requirements
- **Hardware testing** on real ESP32
- **Multiple scenarios** (different cards, volumes, etc.)
- **Web interface testing** on different browsers
- **Serial communication** verification
- **WiFi connectivity** testing

## 📝 Coding Standards

### Code Style
- **Indentation**: 2 spaces (no tabs)
- **Line length**: Maximum 100 characters
- **Comments**: Document complex logic
- **Functions**: Keep functions focused and small
- **Variables**: Use descriptive names

### C++ Guidelines
```cpp
// Use descriptive names
int currentVolume = 15;  // Good
int vol = 15;           // Avoid

// Document functions
/**
 * Reads potentiometer value and maps to volume range
 * @return Volume level (0-25)
 */
int readPotentiometerVolume() {
  // Implementation
}

// Use constants for magic numbers
#define MAX_VOLUME 25    // Good
myDFPlayer.volume(25);   // Avoid magic numbers
```

### Documentation Standards
- **Update README** for user-facing changes
- **Add code comments** for complex logic
- **Document new features** in changelog
- **Include wiring diagrams** for hardware changes

## 🧪 Testing Guidelines

### Hardware Testing
- Test on **real ESP32 hardware**
- Verify **all pin connections**
- Test **different component combinations**
- Check **power consumption**
- Validate **thermal performance**

### Software Testing
- **Compile without warnings**
- **Test all features** manually
- **Verify web interface** functionality
- **Check serial commands**
- **Test error recovery**

### Integration Testing
- **Complete system tests**
- **Multiple RFID cards**
- **Various network conditions**
- **Different browsers**
- **Edge cases** and error conditions

## 📚 Documentation

### Code Documentation
- **Inline comments** for complex logic
- **Function headers** with parameters and returns
- **Pin definitions** clearly labeled
- **Configuration options** explained

### User Documentation
- **Update README** for new features
- **Add troubleshooting** entries
- **Include examples** for new functionality
- **Maintain hardware guides**

## 🏷️ Version Control

### Commit Messages
Follow conventional commits format:
```
type(scope): description

feat(volume): add potentiometer support
fix(wifi): resolve connection timeout
docs(readme): update hardware requirements
refactor(audio): improve DFPlayer error handling
```

### Branch Naming
- `feature/feature-name` - New features
- `fix/bug-description` - Bug fixes
- `docs/documentation-update` - Documentation only
- `refactor/code-improvement` - Code refactoring

### Pull Request Guidelines
1. **Clear title** and description
2. **Reference issues** if applicable
3. **Include testing notes**
4. **Update documentation**
5. **Squash commits** if needed

## 🔍 Review Process

### What We Look For
- **Code quality** and style adherence
- **Hardware compatibility**
- **Documentation completeness**
- **Testing thoroughness**
- **Performance impact**

### Review Timeline
- **Initial response**: Within 48 hours
- **Full review**: Within 1 week
- **Follow-up**: As needed for revisions

## 🎯 Areas for Contribution

### High Priority
- **Battery management** features
- **Mobile app** development
- **Additional audio formats** support
- **Advanced web interface** features
- **Performance optimizations**

### Medium Priority
- **Multi-language support**
- **Custom playlist management**
- **Audio equalizer** controls
- **Scheduled playback** features
- **User authentication**

### Documentation
- **Video tutorials**
- **PCB design files**
- **3D printable enclosures**
- **Alternative hardware** guides
- **Troubleshooting** improvements

### Community
- **Example projects**
- **Use case documentation**
- **Forum participation**
- **Bug triage** assistance
- **New user support**

## 📞 Getting Help

### Development Questions
- **Open discussion** on GitHub
- **Check existing documentation**
- **Ask specific questions** with context
- **Share code snippets** when relevant

### Hardware Issues
- **Check troubleshooting guide** first
- **Include hardware photos** if helpful
- **Describe symptoms** clearly
- **List component specifications**

## 📜 Code of Conduct

### Our Standards
- **Be respectful** and inclusive
- **Help others learn** and grow
- **Focus on constructive** feedback
- **Celebrate contributions** of all sizes

### Unacceptable Behavior
- Harassment or discrimination
- Trolling or inflammatory comments
- Personal attacks
- Spam or off-topic content

## 📄 License

By contributing, you agree that your contributions will be licensed under the MIT License.

## 🙏 Recognition

Contributors will be recognized in:
- **CHANGELOG.md** for significant contributions
- **README.md** contributors section
- **Release notes** for feature additions
- **Project documentation** where appropriate

Thank you for helping make the ESP32 RFID Jukebox better for everyone! 🎵
