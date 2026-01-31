# 🫁 cbreath  
*A calm, terminal-based breathing guide*

> Slow down — without leaving your terminal.

`cbreath` is a minimal, distraction-free CLI breathing guide designed for developers, engineers, and terminal-heavy users who want a moment of calm **without breaking flow**.

It runs entirely in your terminal, uses smooth visual breathing cues, and supports scientifically grounded breathing patterns to help regulate stress and focus.

## ✨ Why cbreath?

Modern work often keeps us in a **sympathetic “fight-or-flight” state** — fast breathing, elevated heart rate, mental noise.

Slow, controlled breathing does the opposite.

### 🧠 The science (briefly)

Extended exhales and breath holds stimulate the **parasympathetic nervous system**, particularly via the **vagus nerve**.

This lowers heart rate, reduces cortisol, and signals safety to the brain.

In short:

**Slower breathing → calmer nervous system → clearer thinking**

`cbreath` turns this into a practical, repeatable habit — right where you already work.

## 🚀 Features

- 🌿 Clean, calming terminal visuals  
- ⏱ Multiple proven breathing modes  
- ⏸ Pause / resume anytime  
- 🔊 Optional sound cues  
- 🛠 Manual & configurable timing  
- 🧘 Gentle end-of-session reflection screen  

No accounts.  
No tracking.  
No noise.


## 📦 Installation

### One-line install (recommended)

curl -fsSL https://raw.githubusercontent.com/AkashKamatCreates/cbreath/main/install.sh | bash

After installation, restart your terminal or run:

hash -r

Then run:

cbreath

## 🧭 Usage

cbreath [MODE]  
cbreath manual TOTAL INHALE HOLD_IN EXHALE HOLD_OUT  
cbreath TOTAL INHALE HOLD_IN EXHALE HOLD_OUT  
cbreath --help

## 🌬 Breathing Modes

box        Inhale 4 · Hold 4 · Exhale 4 · Hold 4  
478        Inhale 4 · Hold 7 · Exhale 8  
relax      Inhale 5 · Hold 2 · Exhale 7 · Hold 3  
calm       Inhale 6 · Hold 2 · Exhale 8 · Hold 2  
focus      Inhale 4 · Exhale 6  
sleep      Inhale 4 · Hold 7 · Exhale 8  
energize   Inhale 3 · Exhale 3  

Example:

cbreath box



## 🛠 Manual Mode

Create your own breathing pattern:

cbreath manual TOTAL INHALE HOLD_IN EXHALE HOLD_OUT

Example:

cbreath manual 300 4 4 6 2

## ⌨ Key Controls

Space   Pause / Resume  
q       Quit

## ⚙ Configuration (optional)

You may create a config file at:

~/.cbreathrc   (recommended)  
./cbreath.conf (local override)

Example:

total_time=600  
preset=box  

# Or custom timings:
# inhale=4
# hold_in=4
# exhale=4
# hold_out=4

sound=off

Command-line arguments always override config values.


## 🧘 End-of-Session Experience

At the end of each session, `cbreath` gently transitions to a quiet reflection screen:

“Take a moment.”  
“Notice how you feel.”


## 🧑‍💻 Author

Written by Akash Kamat

Built with care for people who spend a lot of time in terminals — and sometimes forget to breathe.

## 🤍 Philosophy

Tools don’t need to be loud to be powerful.  
Calm is a feature.

If `cbreath` helped you pause — even briefly — it did its job.
